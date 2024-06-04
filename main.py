import telebot
import subprocess
from telebot import types

# '
# Создайте объект бота, передав токен вашего бота
bot = telebot.TeleBot('6422487476:AAEWMEiSITFf9DHg9p79FJFumZi1WYq5pAg')

# Словарь для хранения логинов и паролей администраторов
admin_credentials = {
    'admin1': 'password1',
    'admin2': 'password2'
}

# Словарь для хранения логинов и паролей судей
judge_credentials = {
    'judge1': 'password1',
    'judge2': 'password2'
}

# Словарь с группами для судей
judge_groups = {
    'judge1': ['Группа 1', 'Группа 2'],
    'judge2': ['Группа 3', 'Группа 4']
}


# Обработчик команды /start
@bot.message_handler(commands=['start'])
def handle_start(message):
    bot.send_message(message.chat.id,
                     'Добро пожаловать! Данный бот помогает быстро и без ошибок проводить соревнования, пожалуйста выберите свою роль:',
                     reply_markup=role_markup())


# Создание разметки клавиатуры для выбора роли
def role_markup():
    markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
    btn_admin = types.KeyboardButton('Администратор')
    btn_judge = types.KeyboardButton('Судья')
    btn_parent = types.KeyboardButton('Родитель')
    markup.add(btn_admin, btn_judge, btn_parent)
    return markup


# Обработчик выбора роли
@bot.message_handler(func=lambda message: message.text in ['Администратор', 'Судья', 'Родитель'])
def handle_role(message):
    role = message.text
    bot.send_message(message.chat.id, f'Вы выбрали роль: {role}')

    if role == 'Администратор':
        ask_login_admin(message.chat.id)

    elif role == 'Судья':
        ask_judge_credentials(message.chat.id)


# Функция запроса логина и пароля для судьи
def ask_judge_credentials(chat_id):
    bot.send_message(chat_id, 'Пожалуйста, введите логин судьи:')
    bot.register_next_step_handler_by_chat_id(chat_id, ask_judge_password)


def ask_judge_password(message):
    judge_login = message.text
    chat_id = message.chat.id
    if judge_login in judge_credentials:
        bot.send_message(chat_id, 'Пожалуйста, введите пароль:')
        bot.register_next_step_handler_by_chat_id(chat_id,
                                                  lambda message: check_judge_credentials(message, judge_login))
    else:
        bot.send_message(chat_id, 'Неверный логин. Попробуйте снова:')
        ask_judge_credentials(chat_id)


def check_judge_credentials(message, judge_login):
    chat_id = message.chat.id
    password = message.text
    if judge_credentials.get(judge_login) == password:
        show_judge_groups(chat_id, judge_login)
    else:
        bot.send_message(chat_id, 'Неверный пароль. Попробуйте снова:')
        ask_judge_password(message)


def show_judge_groups(chat_id, judge_login):
    groups = judge_groups.get(judge_login, [])
    if groups:
        markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
        for group in groups:
            btn_group = types.KeyboardButton(group)
            markup.add(btn_group)
        bot.send_message(chat_id, 'Выберите группу:', reply_markup=markup)
    else:
        bot.send_message(chat_id, 'Для данного судьи не найдены группы. Пожалуйста, обратитесь к администратору.')


# Функция запроса логина и пароля
def ask_login_admin(chat_id):
    bot.send_message(chat_id, 'Пожалуйста, введите логин:')
    bot.register_next_step_handler_by_chat_id(chat_id, ask_password)


def ask_password(message):
    login = message.text
    chat_id = message.chat.id
    if login in admin_credentials:
        bot.send_message(chat_id, 'Пожалуйста, введите пароль:')
        bot.register_next_step_handler_by_chat_id(chat_id, lambda message: check_credentials(message, login))
    else:
        bot.send_message(chat_id, 'Неверный логин. Попробуйте снова:')
        ask_login_admin(chat_id)


def check_credentials(message, login):
    chat_id = message.chat.id
    password = message.text
    if admin_credentials.get(login) == password:
        show_admin_options(chat_id)
    else:
        bot.send_message(chat_id, 'Неверный пароль. Попробуйте снова:')
        ask_password(message)


def show_admin_options(chat_id):
    markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
    btn_get_list = types.KeyboardButton('Получить список участников')
    btn_send_list = types.KeyboardButton('Отправить список участников')
    btn_get_winner = types.KeyboardButton('Определить победителя')
    btn_get_group = types.KeyboardButton('Получить группу')
    btn_add_result = types.KeyboardButton('Изменить результат участника')
    btn_sort_into_group = types.KeyboardButton('Рассортировать участников')
    btn_clear_database = types.KeyboardButton('Очистить базу данных')
    markup.add(btn_get_list, btn_send_list, btn_get_winner, btn_get_group, btn_add_result, btn_sort_into_group,
               btn_clear_database)
    bot.send_message(chat_id, 'Выберите действие:', reply_markup=markup)


# Обработчик команд для админских действий
@bot.message_handler(func=lambda message: message.text in [
    'Получить список участников', 'Отправить список участников', 'Определить победителя',
    'Рассортировать участников', 'Получить группу', 'Изменить результат участника', 'Очистить базу данных'
])
def handle_admin_action(message):
    if message.text == 'Отправить список участников':
        bot.send_message(message.chat.id, 'Отправьте, пожалуйста, список')
        bot.register_next_step_handler(message, handle_file_upload)
    elif message.text == 'Определить победителя':
        bot.send_message(message.chat.id, 'В какой дисциплине вы хотите определить победителя?')
        bot.register_next_step_handler(message, get_discipline)
    elif message.text == 'Рассортировать участников':
        sort_into_groups()
        bot.send_message(message.chat.id, 'Участники успешно рассортированы.')
    elif message.text == 'Получить группу':
        bot.send_message(message.chat.id, 'Группу под каким номером вы хотите получить?')
        bot.register_next_step_handler(message, get_group_number)
    elif message.text == 'Изменить результат участника':
        change_result(message)
    elif message.text == 'Очистить базу данных':
        bot.send_message(message.chat.id, 'Вы уверены, что хотите очистить базу данных? Введите "Да" или "Нет".')
        bot.register_next_step_handler(message, confirm_clear_database)


def change_result(message):
    bot.send_message(message.chat.id, 'Введите имя и фамилию участника')
    bot.register_next_step_handler(message, get_name)


def get_name(message):
    name = message.text
    print(f"Получено имя: {name}")  # Отладочное сообщение
    bot.send_message(message.chat.id, 'Введите результат участника')
    bot.register_next_step_handler(message, get_result, name)


def get_result(message, name):
    result = message.text
    print(f"Получен результат: {result} для {name}")  # Отладочное сообщение
    bot.send_message(message.chat.id, 'Введите дисциплину')
    bot.register_next_step_handler(message, add_result, name, result)


def add_result(message, name, result):
    discipline = message.text
    print(f"Получена дисциплина: {discipline} для {name} с результатом {result}")  # Отладочное сообщение
    save_result(message, name, result, discipline)  # Передача message вместо chat_id


def save_result(message, name, result, discipline):
    chat_id = message.chat.id  # Получение chat_id из message
    executable_path = "D:/addParticipants/cmake-build-debug/addParticipants.exe"

    try:
        name_str = str(name)
        result_str = str(result)
        discipline_str = str(discipline)
        process = subprocess.run([executable_path, name_str, "addResult", result_str, discipline_str],
                                 stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE, text=True, encoding='utf-8')

        if process.returncode != 0:
            error_message = (f"Ошибка при выполнении команды:\n"
                             f"STDOUT: {process.stdout}\n"
                             f"STDERR: {process.stderr}\n"
                             f"Return code: {process.returncode}")
            print(error_message)
            bot.send_message(chat_id, f'Результат для {name} в дисциплине {discipline} не был обновлен.')
        else:
            # Успешное выполнение команды, можно добавить отладочное сообщение
            print(f"Команда выполнена успешно:\nSTDOUT: {process.stdout}")
            bot.send_message(chat_id, f'Результат для {name} в дисциплине {discipline} успешно обновлен.')
    except Exception as e:
        error_message = f"Исключение при выполнении команды: {str(e)}"
        print(error_message)


# Обработчик для подтверждения очистки базы данных
def confirm_clear_database(message):
    if message.text.lower() == 'да':
        clear_database()
        bot.send_message(message.chat.id, 'База данных успешно очищена.')
    elif message.text.lower() == 'нет':
        chat_id = message.chat.id
        show_admin_options(chat_id)
    else:
        bot.send_message(message.chat.id, 'Вы уверены, что хотите очистить базу данных? Введите "Да" или "Нет".')
        bot.register_next_step_handler(message, confirm_clear_database)


# Обработчик для получения дисциплины
def get_discipline(message):
    discipline = message.text
    get_winner(message, discipline)
    bot.send_message(message.chat.id, f'Победитель в дисциплине {discipline} определен.')


# Обработчик для получения номера группы
def get_group_number(message):
    group_number = message.text
    get_group(message,group_number)
    bot.send_message(message.chat.id, f'Группа под номером {group_number} получена.')


def get_group(message, group_number):
    executable_path = "D:/addParticipants/cmake-build-debug/addParticipants.exe"
    try:
        process = subprocess.run([executable_path, group_number, "getGroup"], stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE, text=True, encoding='utf-8')
        bot.send_message(message.chat.id, process.stdout)

        if process.returncode != 0:
            print("STDOUT:", process.stdout)
            print("STDERR:", process.stderr)
            print("Return code:", process.returncode)

    except Exception as e:
        print("Exception:", str(e))


def sort_into_groups():
    executable_path = "D:/addParticipants/cmake-build-debug/addParticipants.exe"
    try:
        process = subprocess.run([executable_path, ' ', "sortIntoGroups"], stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE, text=True, encoding='utf-8')

        if process.returncode != 0:
            print("STDOUT:", process.stdout)
            print("STDERR:", process.stderr)
            print("Return code:", process.returncode)

    except Exception as e:
        print("Exception:", str(e))


def clear_database():
    executable_path = "D:/addParticipants/cmake-build-debug/addParticipants.exe"
    try:
        process = subprocess.run([executable_path, '', "clearDatabase"], stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE, text=True, encoding='utf-8')

        if process.returncode != 0:
            print("STDOUT:", process.stdout)
            print("STDERR:", process.stderr)
            print("Return code:", process.returncode)

    except Exception as e:
        print("Exception:", str(e))


def get_winner(message, discipline):
    executable_path = "D:/addParticipants/cmake-build-debug/addParticipants.exe"
    try:
        process = subprocess.run([executable_path, discipline, "getWinner"], stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE, text=True, encoding='utf-8')
        bot.send_message(message.chat.id, process.stdout)
        if process.returncode != 0:
            print("STDOUT:", process.stdout)
            print("STDERR:", process.stderr)
            print("Return code:", process.returncode)

    except Exception as e:
        print("Exception:", str(e))


# Обработчик загрузки файла
def handle_file_upload(message):
    if message.content_type == 'document':
        file_info = bot.get_file(message.document.file_id)
        downloaded_file = bot.download_file(file_info.file_path)

        # Сохранение файла
        with open('received_file.txt', 'wb') as new_file:
            new_file.write(downloaded_file)

        bot.send_message(message.chat.id, 'Файл получен, обработка...')
        add_participants('received_file.txt')
        bot.send_message(message.chat.id, 'Файл обработан.')
    else:
        bot.send_message(message.chat.id, 'Пожалуйста, отправьте .txt файл.')


# Функция для обработки файла
def add_participants(file_path):
    executable_path = "D:/addParticipants/cmake-build-debug/addParticipants.exe"
    try:
        result = subprocess.run([executable_path, file_path, "addParticipants"], stdin=subprocess.PIPE,
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, encoding='utf-8')

        if result.returncode != 0:
            print("STDOUT:", result.stdout)
            print("STDERR:", result.stderr)
            print("Return code:", result.returncode)

    except FileNotFoundError:
        print("Error: The specified executable or file path does not exist.")
    except PermissionError:
        print("Error: Permission denied when trying to execute the file.")
    except Exception as e:
        print("Exception:", str(e))


# Обработка остальных сообщений
@bot.message_handler(func=lambda message: True)
def handle_other(message):
    bot.send_message(message.chat.id, 'Пожалуйста, используйте кнопки для выбора роли.')


# Запуск бота
bot.polling(none_stop=True)
