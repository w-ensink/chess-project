
/** Written by Wouter Ensink */


#include <shallow_blue_interface.hpp>
#include <juce_core/juce_core.h>
#include <juce_osc/juce_osc.h>
#include "utils.h"
//#include <zconf.h>

auto evaluateCurrentPosition()
{
    auto score                  = Eval::evaluate (Uci::board, WHITE);
    auto hasBishopPair          = Eval::hasBishopPair (Uci::board, WHITE);
    auto numRooksOpenFiles      = Eval::rooksOnOpenFiles (Uci::board, WHITE);
    auto mobility               = Eval::evaluateMobility (Uci::board, GamePhase::ENDGAME, WHITE);
    auto numPassedPawns         = Eval::passedPawns (Uci::board, WHITE);
    auto numDoubledPawns        = Eval::doubledPawns (Uci::board, WHITE);
    auto numIsolatedPawns       = Eval::isolatedPawns (Uci::board, WHITE);
    auto numPawnsShieldingKing  = Eval::pawnsShieldingKing (Uci::board, WHITE);

    print ("score:", score);
    print ("has bishop pair:", hasBishopPair);
    print ("num rooks on open file:", numRooksOpenFiles);
    print ("mobility:", mobility);
    print ("num passed pawns:", numPassedPawns);
    print ("num doubled pawns:", numDoubledPawns);
    print ("num isolated pawns:", numIsolatedPawns);
    print ("num pawns shielding king:", numPawnsShieldingKing);
}


auto sendStats(juce::OSCSender& sender)
{
    auto send = [&sender] (juce::StringRef colour, auto address, auto message)
    {
        auto m = juce::OSCMessage (colour + address);
        m.addInt32 (message);
        sender.send (m);
    };

    for (auto c = 0; c < 2; ++c)
    {
        auto colour = juce::String {c == 0 ? "/white" : "/black"};

        auto score                  = Eval::evaluate (Uci::board, c == 0 ? WHITE : BLACK);
        auto hasBishopPair          = Eval::hasBishopPair (Uci::board, c == 0 ? WHITE : BLACK);
        auto numRooksOpenFiles      = Eval::rooksOnOpenFiles (Uci::board, c == 0 ? WHITE : BLACK);
        auto mobility               = Eval::evaluateMobility (Uci::board, GamePhase::ENDGAME, c == 0 ? WHITE : BLACK);
        auto numPassedPawns         = Eval::passedPawns (Uci::board, c == 0 ? WHITE : BLACK);
        auto numDoubledPawns        = Eval::doubledPawns (Uci::board, c == 0 ? WHITE : BLACK);
        auto numIsolatedPawns       = Eval::isolatedPawns (Uci::board, c == 0 ? WHITE : BLACK);
        auto numPawnsShieldingKing  = Eval::pawnsShieldingKing (Uci::board, c == 0 ? WHITE : BLACK);

        send (colour, "/score", score);
        send (colour, "/has_bishop_pair", (int) hasBishopPair);
        send (colour, "/num_rooks_open_files", numRooksOpenFiles);
        send (colour, "/mobility", mobility);
        send (colour, "/num_passed_pawns", numPassedPawns);
        send (colour, "/num_doubled_pawns", numDoubledPawns);
        send (colour, "/num_isolated_pawns", numIsolatedPawns);
        send (colour, "/num_pawns_shielding_king", numPawnsShieldingKing);
    }
}


auto simulateChessGame (const std::vector<std::string_view>& moves)
{
    auto oscSender = juce::OSCSender();
    oscSender.connect ("127.0.0.1", 5000);
    std::cout << "OSC Sending to port 5000\n";

    eon::initChessEngine();
    Uci::board.setToStartPos();

    for (auto move : moves)
    {
        if (! eon::attemptMove (move))
            return false;

        sendStats (oscSender);

        //sleep (rand() % 5 + 1);
    }

    return true;
}


using RealtimeCallback = juce::OSCReceiver::Listener<juce::OSCReceiver::RealtimeCallback>;


class ChessSimulator    : public RealtimeCallback,
                          private juce::OSCReceiver,
                          private juce::OSCSender
{
public:
    ChessSimulator (int sending, int receiving)
    {
        if (! juce::OSCReceiver::connect (receiving))
            std::cout << "Error connecting receiver\n";

        if (! juce::OSCSender::connect ("127.0.0.1", sending))
            std::cout << "Error connecting sender\n";

        juce::OSCReceiver::addListener (this);
        eon::initChessEngine();
        Uci::board.setToStartPos();
        std::cout << "init done\n";
    }

    void oscMessageReceived (const juce::OSCMessage& message) override
    {
        std::cout << "Received message on address: " << message.getAddressPattern().toString() << ": ";

        for (const auto& arg : message)
        {
            if      (arg.isInt32())   std::cout << arg.getInt32();
            else if (arg.isString())  std::cout << arg.getString();
            else if (arg.isFloat32()) std::cout << arg.getFloat32();

            std::cout << " ";
        }

        std::cout << std::endl;

        if (message.getAddressPattern().toString() == "/m")
        {
            auto move = message.begin()->getString().toStdString();

            if (! eon::attemptMove (move))
            {
                auto m = juce::OSCMessage(juce::OSCAddressPattern ("/move_fail"));
                m.addString ("failed_move");
                send (m);
            }
            else
            {
                sendStats (*this);
            }
        }
    }
};


//==============================================================================
class ConsoleApplication : public juce::JUCEApplicationBase
{
public:
    const juce::String getApplicationName() override    { return "ConsoleApplication"; }
    const juce::String getApplicationVersion() override { return "0.0.1"; }

    void initialise (const juce::String& /*commandLineParameters*/) override {}

    void unhandledException (const std::exception*, const juce::String &, int) override {}
    bool moreThanOneInstanceAllowed() override { return false; }
    void anotherInstanceStarted (const juce::String&) override {}
    void systemRequestedQuit() override {}
    void resumed() override {}
    void suspended() override {}

    void shutdown() override {}

    ChessSimulator sim {5000, 6000};
};

START_JUCE_APPLICATION (ConsoleApplication)
