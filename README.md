This is a bot for organizing competitions in various disciplines. It is implemented as a Telegram bot in Python with a C++ backend for the PostgreSQL database.
The program supports adding and removing participants and their results from the database, distributing participants into groups by streams, and summarizing the competition results, assigning each participant their respective rank.
It also supports saving the database results to an Excel .csv file. Additionally, this database saving feature can be used if the message sent by the bot is too large for Telegram to handle.

C++ Requirements: 
PostgreSQL libreary - https://github.com/postgres/postgres/tree/master 
Spdlog - https://github.com/gabime/spdlog

This bot has 4 access rights: administrator, judge, team leader, parent.

Administrator can add participants and results for them, determine the winners, save database in .csv file and clear database.
Admin credentials: 
login: admin1, admin2.
password: password1, password2.

Judge can add results and check participant's groups.
Judge credentials: 
login: judge1, judge2.
password: password1, password2.

Team leader can check participant's groups only.
Team leader credentials: 
login: teamleader1, teamleader2.
password: password1, password2.

Parents can add participants.
Parent hasn't credentials.
