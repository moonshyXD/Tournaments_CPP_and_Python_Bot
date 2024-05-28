import telebot
from telebot import types

#'
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
    bot.send_message(message.chat.id, 'Добро пожаловать! Данный бот помогает быстро и без ошибок проводить соревнования, пожалуйста выберите свою роль:', reply_markup=role_markup())


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
        bot.register_next_step_handler_by_chat_id(chat_id, lambda message: check_judge_credentials(message, judge_login))
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
    btn_get_list = types.KeyboardButton('Получить список')
    btn_send_list = types.KeyboardButton('Отправить список')
    markup.add(btn_get_list, btn_send_list)
    bot.send_message(chat_id, 'Выберите действие:', reply_markup=markup)


# Обработка остальных сообщений
@bot.message_handler(func=lambda message: True)
def handle_other(message):
    bot.send_message(message.chat.id, 'Пожалуйста, используйте кнопки для выбора роли.')


# Запуск бота
bot.polling(none_stop=True)