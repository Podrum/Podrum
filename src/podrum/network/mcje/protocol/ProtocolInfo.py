"""
*  ____           _
* |  _ \ ___   __| |_ __ _   _ _ __ ___
* | |_) / _ \ / _` | '__| | | | '_ ` _ \
* |  __/ (_) | (_| | |  | |_| | | | | | |
* |_|   \___/ \__,_|_|   \__,_|_| |_| |_|
*
* Licensed under the Mozilla Public License, Version 2.
* Permissions of this weak copyleft license are conditioned on making
* available source code of licensed files and modifications of those files 
* under the same license (or in certain cases, one of the GNU licenses).
* Copyright and license notices must be preserved. Contributors
* provide an express grant of patent rights. However, a larger work
* using the licensed work may be distributed under different terms and without 
* source code for files added in the larger work.
"""

class ProtocolInfo:
    # Versions info
    CURRENT_PROTOCOL = 753
    VERSION = "v1.16.3"
    VERSION_NETWORK = "1.16.3"

    # Handshake Protocol info
    HANDSHAKING = 0x00
    LEGACY_SERVER_LIST = 0xfe
    RESPONSE = 0x00
    PONG = 0x01
    REQUEST = 0x00
    PING = 0x01

    # Login Protocol info
    DISCONNECT = 0x00
    ENCRYPTION_REQUEST = 0x01
    LOGIN_SUCCESS = 0x02
    SET_COMPRESSION = 0x03
    LOGIN_PLUGIN_REQUEST = 0x04
    LOGIN_START = 0x00
    ENCRYPTION_RESPONSE = 0x01
    LOGIN_PLUGIN_RESPONSE = 0x02

    # Play Protocol info
    # Play Clientbound
    SPAWN_ENTITY = 0x00
    SPAWN_EXPERIENCE_ORB = 0x01
    SPAWN_LIVING_ENTITY = 0x02
    SPAWN_PAINTING = 0x03
    SPAWN_PLAYER = 0x04
    ENTITY_ANIMATION = 0x05
    STATISTICS = 0x06
    ACKNOWLEDGE_PLAYER_DIGGING = 0x07
    BLOCK_BREAK_ANIMATION = 0x08
    BLOCK_ENTITY_DATA = 0x09
    BLOCK_ACTION = 0x0A
    BLOCK_CHANGE = 0x0B
    BOSS_BAR = 0x0C
    SERVER_DIFFICULTY = 0x0D
    CLIENTBOUND_CHAT_MESSAGE = 0x0E
    CLIENTBOUND_TAB_COMPLETE = 0xF
    DECLARE_COMMANDS = 0x10
    CLIENTBOUND_WINDOW_CONFIRMATION = 0x11
    CLIENTBOUND_CLOSE_WINDOW = 0x12
    WINDOW_ITEMS = 0x13
    WINDOW_PROPERTY = 0x14
    SET_SLOT = 0x15
    SET_COOLDOWN = 0x16
    CLIENTBOUND_PLUGIN_MESSAGE = 0x17
    NAMED_SOUND_EFFECT = 0x18
    PLAY_DISCONNECT = 0x19
    ENTITY_STATUS = 0x1A
    EXPLOSION = 0x1B
    UNLOAD_CHUNK = 0x1C
    CHANGE_GAME_STATE = 0x1D
    OPEN_HORSE_WINDOW = 0x1E
    CLIENTBOUND_KEEP_ALIVE = 0x1F
    CHUNK_DATA = 0x20
    EFFECT = 0x21
    PARTICLE = 0x22
    UPDATE_LIGHT = 0x23
    JOIN_GAME = 0x24
    MAP_DATA = 0x25
    TRADE_LIST = 0x26
    ENTITY_POSITION = 0x27
    ENTITY_POSITION_AND_ROTATION = 0x28
    ENTITY_ROTATION = 0x29
    ENTITY_MOVEMENT = 0x2A
    CLIENTBOUND_VEHICLE_MOVE = 0x2B
    OPEN_BOOK = 0x2C
    OPEN_WINDOW = 0x2D
    OPEN_SIGN_EDITOR = 0x2E
    CRAFT_RECIPE_RESPONSE = 0x2F
    CLIENTBOUND_PLAYER_ABILITIES = 0x30
    COMBAT_EVENT = 0x31
    PLAYER_INFO = 0x32
    FACE_PLAYER = 0x33
    PLAYER_POSITION_AND_LOOK = 0x34
    UNLOCK_RECIPES = 0x35
    DESTROY_ENTITIES = 0x36
    REMOVE_ENTITY_EFFECT = 0x37
    RESOURCE_PACK_SEND = 0x38
    RESPAWN = 0x29
    ENTITY_HEAD_LOOK = 0x3A
    MULTI_BLOCK_CHANGE = 0x3B
    SELECT_ADVANCEMENT_TAB = 0x3C
    WORLD_BORDER = 0x3D
    CAMERA = 0x3E
    CLIENTBOUND_HELD_ITEM_CHANGE = 0x3F
    UPDATE_VIEW_POSITION = 0x40
    UPDATE_VIEW_DISTANCE = 0x41
    SPAWN_POSITION = 0x42
    DISPLAY_SCOREBOARD = 0x43
    ENTITY_METADATA = 0x44
    ATTACH_ENTITY = 0x45
    ENTITY_VELOCITY = 0x46
    ENTITY_EQUIPMENT = 0x47
    SET_EXPERIENCE = 0x48
    UPDATE_HEALTH = 0x49
    SCOREBOARD_OBJECTIVE = 0x4A
    SET_PASSENGERS = 0x4B
    TEAMS = 0x4C
    UPDATE_SCORE = 0x4D
    TIME_UPDATE = 0x4E
    TITLE = 0x4F
    ENTITY_SOUND_EFFECT = 0x50
    SOUND_EFFECT = 0x51
    STOP_SOUND = 0x52
    PLAYER_LIST_HEADER_AND_FOOTER = 0x53
    NBT_QUERY_RESPONSE = 0x54
    COLLECT_ITEM = 0x55
    ENTITY_TELEPORT = 0x56
    ADVANCEMENTS = 0x57
    ENTITY_PROPERTIES = 0x58
    ENTITY_EFFECT = 0x59
    DECLARE_RECIPES = 0x5A
    TAGS = 0x5B
    # Play Serverbound
    TELEPORT_CONFIRM = 0x00
    QUERY_BLOCK_NBT = 0x01
    QUERY_ENTITY_NBT = 0x0D
    SET_DIFFICULTY = 0x02
    SERVERBOUND_CHAT_MESSAGE = 0x03
    CLIENT_STATUS = 0x04
    CLIENT_SETTINGS = 0x05
    SERVERBOUND_TAB_COMPLETE = 0x06
    SERVERBOUND_WINDOW_CONFIRMATION = 0x07
    CLICK_WINDOW_BUTTON = 0x08
    CLICK_WINDOW = 0x09
    SERVERBOUND_CLOSE_WINDOW = 0x0A
    SERVERBOUND_PLUGIN_MESSAGE = 0x0B
    EDIT_BOOK = 0x0C
    INTERACT_ENTITY = 0x0E
    GENERATE_STRUCTURE = 0x0F
    SERVERBOUND_KEEP_ALIVE = 0x10
    LOCK_DIFFICULTY = 0x11
    PLAYER_POSITION = 0x12
    PLAYER_POSITION_AND_ROTATION = 0x13
    PLAYER_ROTATION = 0x14
    PLAYER_MOVEMENT = 0x15
    SERVERBOUND_VEHICLE_MOVE = 0x16
    STEER_BOAT = 0x17
    PICK_ITEM = 0x18
    CRAFT_RECIPE_REQUEST = 0x19
    SERVERBOUND_PLAYER_ABILITIES = 0x1A
    PLAYER_DIGGING = 0x1B
    ENTITY_ACTION = 0x1C
    STEER_VEHICLE = 0x1D
    SET_DISPLAYED_RECIPE = 0x1E
    SET_RECIPE_BOOK_STATE = 0x1F
    NAME_ITEM = 0x20
    RESOURCE_PACK_STATUS = 0x21
    ADVANCEMENT_TAB = 0x22
    SELECT_TRADE = 0x23
    SET_BEACON_EFFECT = 0x24
    SERVERBOUND_HELD_ITEM_CHANGE = 0x25
    UPDATE_COMMAND_BLOCK = 0x26
    UPDATE_COMMAND_BLOCK_MINECART = 0x27
    UPDATE_JIGSAW_BLOCK = 0x28
    CREATIVE_INVENTORY_ACTION = 0x29
    UPDATE_STRUCTURE_BLOCK = 0x2A
    UPDATE_SIGN = 0x2B
    ANIMATION = 0x2C
    SPECTATE = 0x2D
    PLAYER_BLOCK_PLACEMENT = 0x2E
    USE_ITEM = 0x2F
