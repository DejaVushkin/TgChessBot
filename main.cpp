#include <tgbot/tgbot.h>
#include "chess_engine.h"
#include <iostream>



using namespace TgBot;
using namespace std;


map<int64_t, ChessEngine> games;

int main() {
    Bot bot("Token");

    bot.getEvents().onCommand("start", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Привіт! Я шаховий бот. Використовуйте /newgame, щоб почати нову гру.");
        });

    bot.getEvents().onCommand("newgame", [&bot](Message::Ptr message) {
        int64_t userId = message->chat->id;
        games[userId] = ChessEngine();
        bot.getApi().sendMessage(userId, "Нова гра почалася! Надішліть свій хід у форматі 'e2e4'.");
        bot.getApi().sendMessage(userId, games[userId].getBoard());
        });

    bot.getEvents().onNonCommandMessage([&bot](Message::Ptr message) {
        int64_t userId = message->chat->id;

        if (games.find(userId) == games.end()) {
            bot.getApi().sendMessage(userId, "Використовуйте /newgame, щоб почати гру.");
            return;
        }

        std::string userMove = message->text;

        ChessEngine& game = games[userId];
        if (!game.isValidMove(userMove)) {
            bot.getApi().sendMessage(userId, "Некоректний хід. Спробуйте ще раз.");
            return;
        }

        game.makeMove(userMove);

        if (game.isCheckmate()) {
            bot.getApi().sendMessage(userId, "Вітаємо! Ви виграли! Використовуйте /newgame, щоб почати нову гру.");
            games.erase(userId);
            return;
        }

        std::string botMove = game.calculateBestMove();
        game.makeMove(botMove);

        bot.getApi().sendMessage(userId, "Ваш хід виконано. Хід бота: " + botMove);
        bot.getApi().sendMessage(userId, game.getBoard());

        if (game.isCheckmate()) {
            bot.getApi().sendMessage(userId, "На жаль, ви програли. Використовуйте /newgame, щоб почати нову гру.");
            games.erase(userId);
        }
        });

    try {
        cout << "Бот запущено!" << endl;
        TgLongPoll longPoll(bot);
        while (true) {
            longPoll.start();
        }
    }
    catch (const exception& e) {
        cerr << "Помилка: " << e.what() << endl;
    }

    return 0;
}
