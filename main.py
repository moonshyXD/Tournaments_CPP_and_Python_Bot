import telebot
import subprocess
from telebot import types

bot = telebot.TeleBot('your TelegramAPI')

admin_credentials = {
    'admin1': 'password1',
    'admin2': 'password2'
}

judge_credentials = {
    'judge1': 'password1',
    'judge2': 'password2'
}

team_leader_credentials = {
    'teamleader1': 'password1',
    'teamleader2': 'password2'
}
admin_registration = False
judge_registration = False
team_leader_registration = False


@bot.message_handler(commands=['start'])
def handle_start(message):
    bot.send_message(message.chat.id,
                     'Добро пожаловать! Данный бот помогает быстро и без ошибок проводить соревнования, пожалуйста выберите свою роль:',
                     reply_markup=role_markup())


def translate_rus_to_eng(text):
    alphabet_eng = '0123456789ABCDEFGHIJKLMNOPQRSTUVW'
    alphabet_rus = 'АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ'

    translated_text = []

    for char in text.upper():
        if char in alphabet_rus:
            index = alphabet_rus.index(char)
            translated_text.append(alphabet_eng[index])
        else:
            translated_text.append(char)

    return ''.join(translated_text)


def role_markup():
    markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
    btn_admin = types.KeyboardButton('Администратор')
    btn_judge = types.KeyboardButton('Судья')
    btn_team_leader = types.KeyboardButton('Руководитель группы')
    btn_parent = types.KeyboardButton('Родитель')
    markup.add(btn_admin, btn_judge, btn_team_leader, btn_parent)
    return markup


@bot.message_handler(func=lambda message: message.text in ['Администратор', 'Судья', 'Руководитель группы', 'Родитель'])
def handle_role(message):
    role = message.text
    bot.send_message(message.chat.id, f'Вы выбрали роль: {role}')

    if role == 'Администратор':
        ask_login_admin(message.chat.id)

    elif role == 'Судья':
        ask_judge_credentials(message.chat.id)

    elif role == 'Руководитель группы':
        ask_team_leader_credentials(message.chat.id)

    elif role == 'Родитель':
        show_parent_options(message.chat.id)


def back_to_role_markup():
    markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
    btn_back_to_role = types.KeyboardButton('К выбору роли')
    markup.add(btn_back_to_role)
    return markup


def ask_login_admin(chat_id):
    bot.send_message(chat_id, 'Пожалуйста, введите логин', reply_markup=back_to_role_markup())
    bot.register_next_step_handler_by_chat_id(chat_id, ask_password)


def ask_password(message):
    login = message.text
    chat_id = message.chat.id
    back_to_role_markup()
    if login in admin_credentials:
        bot.send_message(chat_id, 'Пожалуйста, введите пароль', reply_markup=back_to_role_markup())
        bot.register_next_step_handler_by_chat_id(chat_id, lambda message: check_credentials(message, login))
    elif message.text == 'К выбору роли':
        handle_start(message)
    else:
        bot.send_message(chat_id, 'Неверный логин. Попробуйте снова', reply_markup=back_to_role_markup())
        ask_login_admin(chat_id)


def check_credentials(message, login):
    chat_id = message.chat.id
    password = message.text
    if admin_credentials.get(login) == password:
        global admin_registration
        admin_registration = True
        show_admin_options(chat_id)
    elif message.text == 'К выбору роли':
        handle_start(message)
    else:
        bot.send_message(chat_id, 'Неверный пароль. Попробуйте снова:', reply_markup=back_to_role_markup())
        ask_password(message)


def show_admin_options(chat_id):
    markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
    btn_get_list = types.KeyboardButton('Получить список участников')
    btn_send_list = types.KeyboardButton('Отправить список участников')
    btn_add_participant = types.KeyboardButton('Добавить участника')
    btn_delete_participant = types.KeyboardButton('Удалить участника')
    btn_get_group = types.KeyboardButton('Получить группу')
    btn_add_result = types.KeyboardButton('Добавить результат участника')
    btn_delete_result = types.KeyboardButton('Удалить результат участника')
    btn_sort_into_group = types.KeyboardButton('Рассортировать участников')
    btn_clear_database = types.KeyboardButton('Очистить базу данных')
    btn_save_csv = types.KeyboardButton('Подвести итоги')
    btn_start = types.KeyboardButton('К выбору роли')
    markup.add(btn_get_list, btn_send_list, btn_add_participant, btn_delete_participant, btn_get_group, btn_add_result,
               btn_delete_result,
               btn_sort_into_group, btn_clear_database, btn_save_csv, btn_start)
    bot.send_message(chat_id, 'Выберите действие:', reply_markup=markup)


@bot.message_handler(func=lambda message: message.text in [
    'Получить список участников', 'Отправить список участников', 'Добавить участника', 'Удалить участника',
    'Рассортировать участников', 'Получить группу', 'Добавить результат участника',
    'Удалить результат участника', 'Очистить базу данных', 'Подвести итоги', 'К выбору роли'
])
def handle_admin_action(message):
    if admin_registration:
        if message.text == 'Отправить список участников':
            operation = 'addParticipants'
            bot.send_message(message.chat.id, 'Отправьте, пожалуйста, список')
            bot.register_next_step_handler(message, handle_file_upload, operation)
        elif message.text == 'Добавить участника':
            operation = 'addParticipant'
            bot.send_message(message.chat.id, 'Отправьте, пожалуйста, имя участника')
            bot.register_next_step_handler(message, admin_get_first_name, operation)
        elif message.text == 'Удалить участника':
            operation = 'deleteParticipant'
            bot.send_message(message.chat.id, 'Отправьте, пожалуйста, имя участника')
            bot.register_next_step_handler(message, admin_get_first_name, operation)
        elif message.text == 'Добавить результат участника':
            operation = 'addResult'
            bot.send_message(message.chat.id, 'Отправьте, пожалуйста, имя участника')
            bot.register_next_step_handler(message, admin_get_first_name, operation)
        elif message.text == 'Удалить результат участника':
            operation = 'deleteResult'
            bot.send_message(message.chat.id, 'Отправьте, пожалуйста, имя участника')
            bot.register_next_step_handler(message, admin_get_first_name, operation)
        elif message.text == 'Рассортировать участников':
            operation = 'sortIntoGroups'
            CPP_backend(message, '', '', '', '', '', '', '', operation)
        elif message.text == 'Получить группу':
            operation = 'getGroup'
            bot.send_message(message.chat.id, 'Группу под каким номером вы хотите получить?')
            bot.register_next_step_handler(message, admin_get_group_number, operation)
        elif message.text == 'Получить список участников':
            operation = 'getParticipants'
            CPP_backend(message, '', '', '', '', '', '', '', operation)
        elif message.text == 'Подвести итоги':
            operation = 'saveToCSV'
            bot.send_message(message.chat.id,
                             'Отправьте путь к файлу, в который вы хотите сохранить базу данных (файл должен быть формата .csv)')
            bot.register_next_step_handler(message, get_path, operation)
        elif message.text == 'Очистить базу данных':
            operation = 'clearDatabase'
            bot.send_message(message.chat.id, 'Вы уверены, что хотите очистить базу данных? Введите "Да" или "Нет".')
            bot.register_next_step_handler(message, confirm_clear_database, operation)
    if message.text == 'К выбору роли':
        handle_start(message)


def get_path(message, operation):
    path = message.text
    CPP_backend(message, '', '', '', '', '', '', path, operation)


def CPP_backend(message, first_name, last_name, age, result, discipline, group_number, path, operation):
    chat_id = message.chat.id
    executable_path = "D:/TournamentBot/cmake-build-debug/TournamentBot.exe"

    try:
        first_name = str(first_name)
        last_name = str(last_name)
        age = str(age)
        result = str(result)
        discipline = str(discipline)
        group_number = str(group_number)
        path = str(path)
        first_name = translate_rus_to_eng(first_name)
        last_name = translate_rus_to_eng(last_name)
        discipline = translate_rus_to_eng(discipline)

        process = subprocess.run(
            [executable_path, first_name, last_name, age, result, discipline, group_number, path, operation],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE, text=True, encoding='utf-8')

        if process.returncode != 0:
            error_message = (f"Ошибка при выполнении команды:\n"
                             f"STDOUT: {process.stdout}\n"
                             f"STDERR: {process.stderr}\n"
                             f"Return code: {process.returncode}")
            print(error_message)
            #            bot.send_message(chat_id, error_message)
            bot.send_message(chat_id, f"Ошибка при выполнении команды")
        else:
            print(f"Команда выполнена успешно:\nSTDOUT: {process.stdout}")
            bot.send_message(chat_id, process.stdout)
            bot.send_message(chat_id, f'Команда была успешно выполнена.')

        if admin_registration:
            show_admin_options(chat_id)
        elif judge_registration:
            show_judge_options(chat_id)
        elif team_leader_registration:
            show_team_leader_options(chat_id)
        else:
            handle_start(message)
    except Exception as e:
        error_message = f"Исключение при выполнении команды: {str(e)}"
        print(error_message)


def admin_get_first_name(message, operation):
    first_name = message.text
    bot.send_message(message.chat.id, 'Введите фамилию участника')
    bot.register_next_step_handler(message, get_last_name, first_name, operation)


def judge_get_first_name(message, operation):
    first_name = message.text
    bot.send_message(message.chat.id, 'Введите фамилию участника')
    bot.register_next_step_handler(message, get_last_name, first_name, operation)


def parent_get_first_name(message, operation):
    first_name = message.text
    bot.send_message(message.chat.id, 'Введите фамилию участника')
    bot.register_next_step_handler(message, get_last_name, first_name, operation)


def get_last_name(message, first_name, operation):
    last_name = message.text
    bot.send_message(message.chat.id, 'Введите год рождения участника')
    bot.register_next_step_handler(message, get_age, first_name, last_name, operation)


def get_age(message, first_name, last_name, operation):
    age = message.text
    if operation == 'addParticipant' or operation == 'deleteParticipant':
        CPP_backend(message, first_name, last_name, age, '', '', '', '', operation)
    else:
        bot.send_message(message.chat.id, 'Введите дисциплину')
        bot.register_next_step_handler(message, get_discipline, first_name, last_name, age, operation)


def get_discipline(message, first_name, last_name, age, operation):
    discipline = message.text
    if operation == 'deleteResult':
        CPP_backend(message, first_name, last_name, age, '', discipline, '', '', operation)
    elif operation == 'addResult':
        bot.send_message(message.chat.id, 'Введите результат участника')
        bot.register_next_step_handler(message, get_result, first_name, last_name, age, discipline, operation)


def get_result(message, first_name, last_name, age, discipline, operation):
    result = message.text
    CPP_backend(message, first_name, last_name, age, result, discipline, '', '', operation)


def confirm_clear_database(message, operation):
    if message.text.lower() == 'да':
        CPP_backend(message, '', '', '', '', '', '', '', operation)
    elif message.text.lower() == 'нет':
        chat_id = message.chat.id
        show_admin_options(chat_id)
    else:
        bot.send_message(message.chat.id, 'Вы уверены, что хотите очистить базу данных? Введите "Да" или "Нет".')
        bot.register_next_step_handler(message, confirm_clear_database, operation)


def admin_get_group_number(message, operation):
    group_number = message.text
    CPP_backend(message, '', '', '', '', '', group_number, '', operation)
    bot.send_message(message.chat.id, f'Группа под номером {group_number} получена.')


def judge_get_group_number(message, operation):
    group_number = message.text
    CPP_backend(message, '', '', '', '', '', group_number, '', operation)
    bot.send_message(message.chat.id, f'Группа под номером {group_number} получена.')


def team_leader_get_group_number(message, operation):
    group_number = message.text
    CPP_backend(message, '', '', '', '', '', group_number, '', operation)
    bot.send_message(message.chat.id, f'Группа под номером {group_number} получена.')


def handle_file_upload(message, operation):
    if message.content_type == 'document':
        file_info = bot.get_file(message.document.file_id)
        downloaded_file = bot.download_file(file_info.file_path)

        # Сохранение файла
        with open('received_file.txt', 'wb') as new_file:
            new_file.write(downloaded_file)

        CPP_backend(message, '', '', '', '', '', '', 'received_file.txt', operation)
    else:
        bot.send_message(message.chat.id, 'Пожалуйста, отправьте .txt файл.')


def ask_judge_credentials(chat_id):
    bot.send_message(chat_id, 'Пожалуйста, введите логин судьи', reply_markup=back_to_role_markup())
    bot.register_next_step_handler_by_chat_id(chat_id, ask_judge_password)


def ask_judge_password(message):
    judge_login = message.text
    chat_id = message.chat.id
    if judge_login in judge_credentials:
        bot.send_message(chat_id, 'Пожалуйста, введите пароль', reply_markup=back_to_role_markup())
        bot.register_next_step_handler_by_chat_id(chat_id,
                                                  lambda message: check_judge_credentials(message, judge_login))
    elif message.text == 'К выбору роли':
        handle_start(message)
    else:
        bot.send_message(chat_id, 'Неверный логин. Попробуйте снова', reply_markup=back_to_role_markup())
        ask_judge_credentials(chat_id)


def check_judge_credentials(message, judge_login):
    chat_id = message.chat.id
    password = message.text
    if judge_credentials.get(judge_login) == password:
        global judge_registration
        judge_registration = True
        show_judge_options(chat_id)
    elif message.text == 'К выбору роли':
        handle_start(message)
    else:
        bot.send_message(chat_id, 'Неверный пароль. Попробуйте снова', reply_markup=back_to_role_markup())
        ask_judge_password(message)


def show_judge_options(chat_id):
    markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
    btn_get_group = types.KeyboardButton('Получить группу.')
    btn_add_result = types.KeyboardButton('Добавить результат участника.')
    btn_delete_result = types.KeyboardButton('Удалить результат участника.')
    btn_start = types.KeyboardButton('К выбору роли')
    markup.add(btn_get_group, btn_add_result, btn_delete_result, btn_start)
    bot.send_message(chat_id, 'Выберите действие:', reply_markup=markup)


@bot.message_handler(func=lambda message: message.text in [
    'Получить группу.', 'Добавить результат участника.', 'Удалить результат участника.', 'К выбору роли'
])
def handle_judge_action(message):
    if judge_registration:
        if message.text == 'Получить группу.':
            operation = 'getGroup'
            bot.send_message(message.chat.id, 'Группу под каким номером вы хотите получить?')
            bot.register_next_step_handler(message, judge_get_group_number, operation)
        elif message.text == 'Добавить результат участника.':
            operation = 'addResult'
            bot.send_message(message.chat.id, 'Отправьте, пожалуйста, имя участника')
            bot.register_next_step_handler(message, judge_get_first_name, operation)
        elif message.text == 'Удалить результат участника.':
            operation = 'deleteResult'
            bot.send_message(message.chat.id, 'Отправьте, пожалуйста, имя участника')
            bot.register_next_step_handler(message, judge_get_first_name, operation)
    if message.text == 'К выбору роли':
        handle_start(message)


def ask_team_leader_credentials(chat_id):
    bot.send_message(chat_id, 'Пожалуйста, введите логин руководителя группы', reply_markup=back_to_role_markup())
    bot.register_next_step_handler_by_chat_id(chat_id, ask_team_leader_password)


def ask_team_leader_password(message):
    team_leader_login = message.text
    chat_id = message.chat.id
    if team_leader_login in team_leader_credentials:
        bot.send_message(chat_id, 'Пожалуйста, введите пароль', reply_markup=back_to_role_markup())
        bot.register_next_step_handler_by_chat_id(chat_id,
                                                  lambda message: check_team_leader_credentials(message,
                                                                                                team_leader_login))
    elif message.text == 'К выбору роли':
        handle_start(message)
    else:
        bot.send_message(chat_id, 'Неверный логин. Попробуйте снова', reply_markup=back_to_role_markup())
        ask_team_leader_credentials(chat_id)


def check_team_leader_credentials(message, team_leader_login):
    chat_id = message.chat.id
    password = message.text
    if team_leader_credentials.get(team_leader_login) == password:
        global team_leader_registration
        team_leader_registration = True
        show_team_leader_options(chat_id)
    elif message.text == 'К выбору роли':
        handle_start(message)
    else:
        bot.send_message(chat_id, 'Неверный пароль. Попробуйте снова', reply_markup=back_to_role_markup())
        ask_team_leader_password(message)


def show_team_leader_options(chat_id):
    markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
    btn_get_group = types.KeyboardButton('Получить грyппу .')
    btn_start = types.KeyboardButton('К выбору роли')
    markup.add(btn_get_group, btn_start)
    bot.send_message(chat_id, 'Выберите действие:', reply_markup=markup)


@bot.message_handler(func=lambda message: message.text in [
    'Получить грyппу .', 'К выбору роли'
])
def handle_team_leader_action(message):
    if team_leader_registration:
        if message.text == 'Получить грyппу .':
            operation = 'getGroup'
            bot.send_message(message.chat.id, 'Группу под каким номером вы хотите получить?')
            bot.register_next_step_handler(message, team_leader_get_group_number, operation)
    if message.text == 'К выбору роли':
        handle_start(message)


def show_parent_options(chat_id):
    markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
    btn_add_participant = types.KeyboardButton('Добавить участника.')
    btn_start = types.KeyboardButton('К выбору роли')
    markup.add(btn_add_participant, btn_start)
    bot.send_message(chat_id, 'Выберите действие:', reply_markup=markup)


@bot.message_handler(func=lambda message: message.text in [
    'Добавить участника.', 'К выбору роли'
])
def handle_parent_action(message):
    if message.text == 'Добавить участника.':
        bot.send_message(message.chat.id, 'Отправьте, пожалуйста, имя участника.')
        operation = 'addParticipant'
        bot.register_next_step_handler(message, parent_get_first_name, operation)
    elif message.text == 'К выбору роли':
        handle_start(message)


# Обработка остальных сообщений
@bot.message_handler(func=lambda message: True)
def handle_other(message):
    bot.send_message(message.chat.id, 'Пожалуйста, используйте кнопки для выбора роли.')


# Запуск бота
bot.polling(none_stop=True)
