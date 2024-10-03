import logging
import telebot
import subprocess
import os
from telebot import types
from Requirements.GeneralMessages import *
from Requirements.Requests import *
from Requirements.Paths import *
from Requirements.Alphabets import *
from Requirements.AuthorizationMessages import *
from Requirements.DebugMessages import *
from Requirements.Operations import *
from Requirements.Buttons import *
from Requirements.TelegramAPI import *
from Requirements.Credentials import *
from Requirements.Commands import *

logging.basicConfig(
    level=logging.DEBUG,
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    handlers=[
        logging.FileHandler('bot_logs.log', mode='a', encoding='utf-8'),
    ]
)

bot = telebot.TeleBot(TELEGRAM_API)

admin_registration = False
judge_registration = False
team_leader_registration = False

logger = logging.getLogger(__name__)


@bot.message_handler(commands=[START])
def handle_start(message: types.Message):
    logger.info("Получена команда /start")
    global admin_registration, judge_registration, team_leader_registration
    admin_registration = False
    judge_registration = False
    team_leader_registration = False
    bot.send_message(message.chat.id, WELCOME_MESSAGE, reply_markup=role_markup())


def translate_rus_to_eng(text: str) -> str:
    logger.debug(f"Перевод текста: {text}")
    alphabet_eng = ENGLISH_ALPHABET
    alphabet_rus = RUSSIAN_ALPHABET

    translated_text = []

    for char in text.upper():
        if char in alphabet_rus:
            index = alphabet_rus.index(char)
            translated_text.append(alphabet_eng[index])
        else:
            translated_text.append(char)

    result = ''.join(translated_text)
    logger.debug(f"Переведенный текст: {result}")
    return result


def role_markup() -> types.ReplyKeyboardMarkup:
    logger.debug("Создание разметки ролей")
    markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
    btn_admin = types.KeyboardButton(BTN_ADMIN)
    btn_judge = types.KeyboardButton(BTN_JUDGE)
    btn_team_leader = types.KeyboardButton(BTN_TEAM_LEADER)
    btn_parent = types.KeyboardButton(BTN_PARENT)
    markup.add(btn_admin, btn_judge, btn_team_leader, btn_parent)
    return markup


@bot.message_handler(func=lambda message: message.text in [BTN_ADMIN, BTN_JUDGE, BTN_TEAM_LEADER, BTN_PARENT])
def handle_role(message: types.Message):
    logger.info(f"Выбрана роль: {message.text}")
    role = message.text
    bot.send_message(message.chat.id, PICKED_ROLE)

    if role == BTN_ADMIN:
        ask_login(message.chat.id)

    elif role == BTN_JUDGE:
        ask_login(message.chat.id)

    elif role == BTN_TEAM_LEADER:
        ask_login(message.chat.id)

    elif role == BTN_PARENT:
        show_options(message.chat.id)


def back_to_role_markup() -> types.ReplyKeyboardMarkup:
    logger.debug("Создание разметки возврата к ролям")
    markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
    btn_back_to_role = types.KeyboardButton(BTN_BACK_TO_ROLE)
    markup.add(btn_back_to_role)
    return markup


def ask_login(chat_id: int):
    logger.info(f"Запрос логина для chat_id: {chat_id}")
    bot.send_message(chat_id, LOGIN_REQUEST, reply_markup=back_to_role_markup())
    bot.register_next_step_handler_by_chat_id(chat_id, ask_password)


def ask_password(message: types.Message):
    logger.info(f"Запрос пароля для пользователя: {message.text}")
    login = message.text
    chat_id = message.chat.id
    back_to_role_markup()

    if login in admin_credentials or login in judge_credentials or login in team_leader_credentials:
        bot.send_message(chat_id, PASSWORD_REQUET, reply_markup=back_to_role_markup())
        bot.register_next_step_handler_by_chat_id(chat_id, lambda message: check_credentials(message, login))
        return
    if message.text == BTN_BACK_TO_ROLE:
        handle_start(message)
        return

    logger.warning(f"Неправильная попытка входа: {login}")
    bot.send_message(chat_id, INVALID_LOGIN, reply_markup=back_to_role_markup())
    ask_login(chat_id)


def check_credentials(message: types.Message, login: str):
    logger.info(f"Проверка учетных данных для пользователя: {login}")
    chat_id = message.chat.id
    password = message.text

    if admin_credentials.get(login) == password:
        global admin_registration
        admin_registration = True
        show_options(chat_id)
        return
    if judge_credentials.get(login) == password:
        global judge_registration
        judge_registration = True
        show_options(chat_id)
        return
    if team_leader_credentials.get(login) == password:
        global team_leader_registration
        team_leader_registration = True
        show_options(chat_id)
        return
    if message.text == BTN_BACK_TO_ROLE:
        handle_start(message)
        return

    logger.warning(f"Неправильная попытка ввода пароля для пользователя: {login}")
    bot.send_message(chat_id, INVALID_PASSWORD, reply_markup=back_to_role_markup())
    ask_password(message)


def show_options(chat_id: int):
    logger.info(f"Отображение опций для chat_id: {chat_id}")
    markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
    btn_get_list = types.KeyboardButton(BTN_GET_LIST)
    btn_send_list = types.KeyboardButton(BTN_SEND_LIST)
    btn_post_participant = types.KeyboardButton(BTN_POST_PARTICIPANT)
    btn_delete_participant = types.KeyboardButton(BTN_DELETE_PARTICIPANT)
    btn_get_group = types.KeyboardButton(BTN_RETRIEVE_GROUP)
    btn_post_result = types.KeyboardButton(BTN_POST_RESULT)
    btn_delete_result = types.KeyboardButton(BTN_DELETE_RESULT)
    btn_sort_into_group = types.KeyboardButton(BTN_SORT_INTO_GROUP)
    btn_clear_database = types.KeyboardButton(BTN_CLEAR_DATABASE)
    btn_save_csv = types.KeyboardButton(BTN_SAVE_CSV)
    btn_start = types.KeyboardButton(BTN_BACK_TO_ROLE)
    buttons = [btn_get_list, btn_send_list, btn_post_participant, btn_delete_participant, btn_get_group,
               btn_post_result, btn_delete_result, btn_sort_into_group, btn_clear_database, btn_save_csv, btn_start]
    distribute_options(buttons, markup)

    bot.send_message(chat_id, CHOOSE_ACTION, reply_markup=markup)


def distribute_options(buttons: list, markup: types.ReplyKeyboardMarkup):
    logger.debug("Распределение опций в зависимости от роли пользователя")
    (btn_get_list, btn_send_list, btn_post_participant, btn_delete_participant, btn_get_group, btn_post_result,
     btn_delete_result, btn_sort_into_group, btn_clear_database, btn_save_csv, btn_start) = buttons
    if admin_registration:
        markup.add(btn_get_list, btn_send_list, btn_post_participant, btn_delete_participant, btn_get_group,
                   btn_post_result, btn_delete_result,
                   btn_sort_into_group, btn_clear_database, btn_save_csv, btn_start)
    elif judge_registration:
        markup.add(btn_get_group, btn_post_result, btn_delete_result, btn_start)
    elif team_leader_registration:
        markup.add(btn_get_group, btn_start)
    else:
        markup.add(btn_post_participant, btn_start)


@bot.message_handler(func=lambda message: message.text in [
    BTN_GET_LIST, BTN_SEND_LIST, BTN_POST_PARTICIPANT, BTN_DELETE_PARTICIPANT,
    BTN_SORT_INTO_GROUP, BTN_RETRIEVE_GROUP, BTN_POST_RESULT,
    BTN_DELETE_RESULT, BTN_CLEAR_DATABASE, BTN_SAVE_CSV, BTN_BACK_TO_ROLE
])
def handle_action(message: types.Message):
    logger.info(f"Обработка действия: {message.text}")
    if admin_registration:
        if message.text == BTN_SEND_LIST:
            operation = LOAD_PARTICIPANTS_FORM_FILE_OPERATION
            bot.send_message(message.chat.id, FILE_REQUEST)
            bot.register_next_step_handler(message, handle_file_upload, operation)
        elif message.text == BTN_DELETE_PARTICIPANT:
            operation = DELETE_PARTICIPANT_OPERATION
            bot.send_message(message.chat.id, FIRST_NAME_REQUEST)
            bot.register_next_step_handler(message, get_first_name, operation)
        elif message.text == BTN_SORT_INTO_GROUP:
            operation = SORT_INTO_GROUPS_OPERATION
            CPP_backend(message, NONE, NONE, NONE, NONE, NONE, NONE, NONE, operation)
        elif message.text == BTN_GET_LIST:
            operation = RETRIEVE_PARTICIPANTS_OPERATION
            CPP_backend(message, NONE, NONE, NONE, NONE, NONE, NONE, NONE, operation)
        elif message.text == BTN_SAVE_CSV:
            operation = SAVE_TO_CSV_OPERATION
            bot.send_message(message.chat.id, CSV_PATH_REQUEST)
            bot.register_next_step_handler(message, get_path, operation)
        elif message.text == BTN_CLEAR_DATABASE:
            operation = CLEAR_DATABASE_OPERATION
            bot.send_message(message.chat.id, CLEAR_DATABASE_CONFIRMATION_REQUEST)
            bot.register_next_step_handler(message, confirm_clear_database, operation)

    if admin_registration or judge_registration:
        if message.text == BTN_POST_RESULT:
            operation = POST_RESULT_OPERATION
            bot.send_message(message.chat.id, FIRST_NAME_REQUEST)
            bot.register_next_step_handler(message, get_first_name, operation)
        elif message.text == BTN_DELETE_RESULT:
            operation = DELETE_RESULT_OPERATION
            bot.send_message(message.chat.id, FIRST_NAME_REQUEST)
            bot.register_next_step_handler(message, get_first_name, operation)

    if admin_registration or judge_registration or team_leader_registration:
        if message.text == BTN_RETRIEVE_GROUP:
            operation = RETRIEVE_GROUP_OPERATION
            bot.send_message(message.chat.id, GROUP_NUMBER_REQUEST)
            bot.register_next_step_handler(message, get_group_number, operation)

    if message.text == BTN_POST_PARTICIPANT:
        operation = POST_PARTICIPANT_OPERATION
        bot.send_message(message.chat.id, FIRST_NAME_REQUEST)
        bot.register_next_step_handler(message, get_first_name, operation)

    if message.text == BTN_BACK_TO_ROLE:
        handle_start(message)


def get_path(message: types.Message, operation: str):
    logger.info(f"Получение пути для операции: {operation}")
    path = message.text
    CPP_backend(message, NONE, NONE, NONE, NONE, NONE, NONE, path, operation)


def CPP_backend(message: types.Message, first_name: types.Message, last_name: types.Message, age: types.Message,
                result: types.Message, discipline: types.Message, group_number: types.Message, path: types.Message,
                operation: str):
    chat_id = message.chat.id
    executable_path = CPP_MAIN_PATH

    arguments = [first_name, last_name, age, result, discipline, group_number, path]
    arguments = [str(argument) for argument in arguments]
    first_name, last_name, age, result, discipline, group_number, path = arguments

    try:
        first_name = translate_rus_to_eng(first_name)
        last_name = translate_rus_to_eng(last_name)
        discipline = translate_rus_to_eng(discipline)

        logger.debug(f"Выполнение backend с аргументами: {arguments}")
        process = subprocess.run(
            [executable_path, first_name, last_name, age, result, discipline, group_number, path, operation],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE, text=True, encoding='utf-8')

        debugging_process(chat_id, process)
        bot_repeat(chat_id, message)
    except Exception as e:
        logger.error(f"Произошла ошибка: {e}")
        bot.send_message(chat_id, EXCEPTION_MESSAGE)


def debugging_process(chat_id: int, process: subprocess.run):
    if process.returncode != 0:
        error_message = (COMMAND_EXECUTION_ERROR,
                         STDOUT_MESSAGE, f"{process.stdout}\n",
                         STDERR_MESSAGE, f"{process.stderr}\n",
                         RETURN_CODE_MESSAGE, f"{process.returncode}")
        logger.error(f"Ошибка при выполнении команды: {error_message}")
        bot.send_message(chat_id, COMMAND_EXECUTION_ERROR)
    else:
        logger.info(f"Команда успешно выполнена: {process.stdout}")
        bot.send_message(chat_id, process.stdout)
        bot.send_message(chat_id, COMMAND_SUCCESS)


def bot_repeat(chat_id: int, message: types.Message):
    global admin_registration, judge_registration, team_leader_registration
    if admin_registration or judge_registration or team_leader_registration:
        show_options(chat_id)
        return

    handle_start(message)


def check_first_name(first_name: str):
    for i in first_name:
        if i == ' ':
            return False
    return True


def check_age(age: str):
    for i in age:
        if '0' >= i >= '9':
            return False
    return True


def get_first_name(message: types.Message, operation: str):
    first_name = message.text
    if not check_first_name(first_name):
        bot.send_message(message.chat.id, ERROR_FIRST_NAME_REQUEST)
        bot.register_next_step_handler(message, get_first_name, operation)
        return
    bot.send_message(message.chat.id, LAST_NAME_REQUEST)
    bot.register_next_step_handler(message, get_last_name, first_name, operation)


def get_last_name(message: types.Message, first_name: types.Message, operation: str):
    last_name = message.text
    bot.send_message(message.chat.id, AGE_REQUEST)
    bot.register_next_step_handler(message, get_age, first_name, last_name, operation)


def get_age(message: types.Message, first_name: types.Message, last_name: types.Message, operation: str):
    age = message.text
    if not check_first_name(age):
        bot.send_message(message.chat.id, AGE_REQUEST)
        bot.register_next_step_handler(message, get_age, first_name, last_name, operation)
        return

    if operation == POST_PARTICIPANT_OPERATION or operation == DELETE_PARTICIPANT_OPERATION:
        CPP_backend(message, first_name, last_name, age, NONE, NONE, NONE, NONE, operation)
        return

    bot.send_message(message.chat.id, DISCIPLINE_REQUEST)
    bot.register_next_step_handler(message, get_discipline, first_name, last_name, age, operation)


def get_discipline(message: types.Message, first_name: types.Message, last_name: types.Message, age: types.Message,
                   operation: str):
    discipline = message.text
    if operation == DELETE_RESULT_OPERATION:
        CPP_backend(message, first_name, last_name, age, NONE, discipline, NONE, NONE, operation)
    elif operation == POST_RESULT_OPERATION:
        bot.send_message(message.chat.id, RESULT_REQUEST)
        bot.register_next_step_handler(message, get_result, first_name, last_name, age, discipline, operation)


def get_result(message: types.Message, first_name: types.Message, last_name: types.Message, age: types.Message,
               discipline: types.Message, operation: str):
    result = message.text
    CPP_backend(message, first_name, last_name, age, result, discipline, NONE, NONE, operation)


def confirm_clear_database(message: types.Message, operation: str):
    if message.text.lower() == YES_REQUEST:
        CPP_backend(message, NONE, NONE, NONE, NONE, NONE, NONE, NONE, operation)
        return
    if message.text.lower() == NO_REQUEST:
        chat_id = message.chat.id
        show_options(chat_id)
        return

    bot.send_message(message.chat.id, CLEAR_DATABASE_CONFIRMATION_REQUEST)
    bot.register_next_step_handler(message, confirm_clear_database, operation)


def get_group_number(message: types.Message, operation: str):
    group_number = message.text
    CPP_backend(message, NONE, NONE, NONE, NONE, NONE, group_number, NONE, operation)
    bot.send_message(message.chat.id, GROUP_RECEIVED, f'{group_number}')


def save_file_and_get_path(message: types.Message) -> str:
    if message.content_type == 'document':
        file_info = bot.get_file(message.document.file_id)
        downloaded_file = bot.download_file(file_info.file_path)

        file_name = message.document.file_name
        file_path = os.path.join(BASE_DIRECTORY, file_name)

        os.makedirs(BASE_DIRECTORY, exist_ok=True)

        with open(file_path, 'wb') as new_file:
            new_file.write(downloaded_file)

        logger.info(f"Файл загружен: {file_path}")

        return file_path
    else:
        logger.warning("Загружен неверный тип файла")
        bot.send_message(message.chat.id, CSV_PATH_REQUEST)
        return ''


def handle_file_upload(message: types.Message, operation: str):
    file_path = save_file_and_get_path(message)
    if file_path:
        CPP_backend(message, NONE, NONE, NONE, NONE, NONE, NONE, file_path, operation)


@bot.message_handler(func=lambda message: True)
def handle_other(message: types.Message):
    logger.info(f"Получено необработанное сообщение: {message.text}")
    bot.send_message(message.chat.id, USE_BUTTONS)


bot.polling(none_stop=True)
