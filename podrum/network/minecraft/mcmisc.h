/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_NETWORK_MINECRAFT_MCMISC_H
#define PODRUM_NETWORK_MINECRAFT_MCMISC_H

#include <stdint.h>
#include <cbinarystream/binary_stream.h>

#define GAME_RULE_BOOLEAN 1
#define GAME_RULE_SIGNED_VAR_INT 2
#define GAME_RULE_FLOAT_LE 3

#define ITEM_EXTRA_DATA_WITH_NBT 0xffff
#define ITEM_EXTRA_DATA_WITHOUT_NBT 0x0000

#define BIOME_DATA_BLOB "CgAKDWJhbWJvb19qdW5nbGUFCGRvd25mYWxsZmZmPwULdGVtcGVyYXR1cmUzM3M/AAoTYmFtYm9vX2p1bmdsZV9oaWxscwUIZG93bmZhbGxmZmY/BQt0ZW1wZXJhdHVyZTMzcz8ACgViZWFjaAUIZG93bmZhbGzNzMw+BQt0ZW1wZXJhdHVyZc3MTD8ACgxiaXJjaF9mb3Jlc3QFCGRvd25mYWxsmpkZPwULdGVtcGVyYXR1cmWamRk/AAoSYmlyY2hfZm9yZXN0X2hpbGxzBQhkb3duZmFsbJqZGT8FC3RlbXBlcmF0dXJlmpkZPwAKGmJpcmNoX2ZvcmVzdF9oaWxsc19tdXRhdGVkBQhkb3duZmFsbM3MTD8FC3RlbXBlcmF0dXJlMzMzPwAKFGJpcmNoX2ZvcmVzdF9tdXRhdGVkBQhkb3duZmFsbM3MTD8FC3RlbXBlcmF0dXJlMzMzPwAKCmNvbGRfYmVhY2gFCGRvd25mYWxsmpmZPgULdGVtcGVyYXR1cmXNzEw9AAoKY29sZF9vY2VhbgUIZG93bmZhbGwAAAA/BQt0ZW1wZXJhdHVyZQAAAD8ACgpjb2xkX3RhaWdhBQhkb3duZmFsbM3MzD4FC3RlbXBlcmF0dXJlAAAAvwAKEGNvbGRfdGFpZ2FfaGlsbHMFCGRvd25mYWxszczMPgULdGVtcGVyYXR1cmUAAAC/AAoSY29sZF90YWlnYV9tdXRhdGVkBQhkb3duZmFsbM3MzD4FC3RlbXBlcmF0dXJlAAAAvwAKD2RlZXBfY29sZF9vY2VhbgUIZG93bmZhbGwAAAA/BQt0ZW1wZXJhdHVyZQAAAD8AChFkZWVwX2Zyb3plbl9vY2VhbgUIZG93bmZhbGwAAAA/BQt0ZW1wZXJhdHVyZQAAAAAAChNkZWVwX2x1a2V3YXJtX29jZWFuBQhkb3duZmFsbAAAAD8FC3RlbXBlcmF0dXJlAAAAPwAKCmRlZXBfb2NlYW4FCGRvd25mYWxsAAAAPwULdGVtcGVyYXR1cmUAAAA/AAoPZGVlcF93YXJtX29jZWFuBQhkb3duZmFsbAAAAD8FC3RlbXBlcmF0dXJlAAAAPwAKBmRlc2VydAUIZG93bmZhbGwAAAAABQt0ZW1wZXJhdHVyZQAAAEAACgxkZXNlcnRfaGlsbHMFCGRvd25mYWxsAAAAAAULdGVtcGVyYXR1cmUAAABAAAoOZGVzZXJ0X211dGF0ZWQFCGRvd25mYWxsAAAAAAULdGVtcGVyYXR1cmUAAABAAAoNZXh0cmVtZV9oaWxscwUIZG93bmZhbGyamZk+BQt0ZW1wZXJhdHVyZc3MTD4AChJleHRyZW1lX2hpbGxzX2VkZ2UFCGRvd25mYWxsmpmZPgULdGVtcGVyYXR1cmXNzEw+AAoVZXh0cmVtZV9oaWxsc19tdXRhdGVkBQhkb3duZmFsbJqZmT4FC3RlbXBlcmF0dXJlzcxMPgAKGGV4dHJlbWVfaGlsbHNfcGx1c190cmVlcwUIZG93bmZhbGyamZk+BQt0ZW1wZXJhdHVyZc3MTD4ACiBleHRyZW1lX2hpbGxzX3BsdXNfdHJlZXNfbXV0YXRlZAUIZG93bmZhbGyamZk+BQt0ZW1wZXJhdHVyZc3MTD4ACg1mbG93ZXJfZm9yZXN0BQhkb3duZmFsbM3MTD8FC3RlbXBlcmF0dXJlMzMzPwAKBmZvcmVzdAUIZG93bmZhbGzNzEw/BQt0ZW1wZXJhdHVyZTMzMz8ACgxmb3Jlc3RfaGlsbHMFCGRvd25mYWxszcxMPwULdGVtcGVyYXR1cmUzMzM/AAoMZnJvemVuX29jZWFuBQhkb3duZmFsbAAAAD8FC3RlbXBlcmF0dXJlAAAAAAAKDGZyb3plbl9yaXZlcgUIZG93bmZhbGwAAAA/BQt0ZW1wZXJhdHVyZQAAAAAACgRoZWxsBQhkb3duZmFsbAAAAAAFC3RlbXBlcmF0dXJlAAAAQAAKDWljZV9tb3VudGFpbnMFCGRvd25mYWxsAAAAPwULdGVtcGVyYXR1cmUAAAAAAAoKaWNlX3BsYWlucwUIZG93bmZhbGwAAAA/BQt0ZW1wZXJhdHVyZQAAAAAAChFpY2VfcGxhaW5zX3NwaWtlcwUIZG93bmZhbGwAAIA/BQt0ZW1wZXJhdHVyZQAAAAAACgZqdW5nbGUFCGRvd25mYWxsZmZmPwULdGVtcGVyYXR1cmUzM3M/AAoLanVuZ2xlX2VkZ2UFCGRvd25mYWxszcxMPwULdGVtcGVyYXR1cmUzM3M/AAoTanVuZ2xlX2VkZ2VfbXV0YXRlZAUIZG93bmZhbGzNzEw/BQt0ZW1wZXJhdHVyZTMzcz8ACgxqdW5nbGVfaGlsbHMFCGRvd25mYWxsZmZmPwULdGVtcGVyYXR1cmUzM3M/AAoOanVuZ2xlX211dGF0ZWQFCGRvd25mYWxsZmZmPwULdGVtcGVyYXR1cmUzM3M/AAoTbGVnYWN5X2Zyb3plbl9vY2VhbgUIZG93bmZhbGwAAAA/BQt0ZW1wZXJhdHVyZQAAAAAACg5sdWtld2FybV9vY2VhbgUIZG93bmZhbGwAAAA/BQt0ZW1wZXJhdHVyZQAAAD8ACgptZWdhX3RhaWdhBQhkb3duZmFsbM3MTD8FC3RlbXBlcmF0dXJlmpmZPgAKEG1lZ2FfdGFpZ2FfaGlsbHMFCGRvd25mYWxszcxMPwULdGVtcGVyYXR1cmWamZk+AAoEbWVzYQUIZG93bmZhbGwAAAAABQt0ZW1wZXJhdHVyZQAAAEAACgptZXNhX2JyeWNlBQhkb3duZmFsbAAAAAAFC3RlbXBlcmF0dXJlAAAAQAAKDG1lc2FfcGxhdGVhdQUIZG93bmZhbGwAAAAABQt0ZW1wZXJhdHVyZQAAAEAAChRtZXNhX3BsYXRlYXVfbXV0YXRlZAUIZG93bmZhbGwAAAAABQt0ZW1wZXJhdHVyZQAAAEAAChJtZXNhX3BsYXRlYXVfc3RvbmUFCGRvd25mYWxsAAAAAAULdGVtcGVyYXR1cmUAAABAAAoabWVzYV9wbGF0ZWF1X3N0b25lX211dGF0ZWQFCGRvd25mYWxsAAAAAAULdGVtcGVyYXR1cmUAAABAAAoPbXVzaHJvb21faXNsYW5kBQhkb3duZmFsbAAAgD8FC3RlbXBlcmF0dXJlZmZmPwAKFW11c2hyb29tX2lzbGFuZF9zaG9yZQUIZG93bmZhbGwAAIA/BQt0ZW1wZXJhdHVyZWZmZj8ACgVvY2VhbgUIZG93bmZhbGwAAAA/BQt0ZW1wZXJhdHVyZQAAAD8ACgZwbGFpbnMFCGRvd25mYWxszczMPgULdGVtcGVyYXR1cmXNzEw/AAobcmVkd29vZF90YWlnYV9oaWxsc19tdXRhdGVkBQhkb3duZmFsbM3MTD8FC3RlbXBlcmF0dXJlmpmZPgAKFXJlZHdvb2RfdGFpZ2FfbXV0YXRlZAUIZG93bmZhbGzNzEw/BQt0ZW1wZXJhdHVyZQAAgD4ACgVyaXZlcgUIZG93bmZhbGwAAAA/BQt0ZW1wZXJhdHVyZQAAAD8ACg1yb29mZWRfZm9yZXN0BQhkb3duZmFsbM3MTD8FC3RlbXBlcmF0dXJlMzMzPwAKFXJvb2ZlZF9mb3Jlc3RfbXV0YXRlZAUIZG93bmZhbGzNzEw/BQt0ZW1wZXJhdHVyZTMzMz8ACgdzYXZhbm5hBQhkb3duZmFsbAAAAAAFC3RlbXBlcmF0dXJlmpmZPwAKD3NhdmFubmFfbXV0YXRlZAUIZG93bmZhbGwAAAA/BQt0ZW1wZXJhdHVyZc3MjD8ACg9zYXZhbm5hX3BsYXRlYXUFCGRvd25mYWxsAAAAAAULdGVtcGVyYXR1cmUAAIA/AAoXc2F2YW5uYV9wbGF0ZWF1X211dGF0ZWQFCGRvd25mYWxsAAAAPwULdGVtcGVyYXR1cmUAAIA/AAoLc3RvbmVfYmVhY2gFCGRvd25mYWxsmpmZPgULdGVtcGVyYXR1cmXNzEw+AAoQc3VuZmxvd2VyX3BsYWlucwUIZG93bmZhbGzNzMw+BQt0ZW1wZXJhdHVyZc3MTD8ACglzd2FtcGxhbmQFCGRvd25mYWxsAAAAPwULdGVtcGVyYXR1cmXNzEw/AAoRc3dhbXBsYW5kX211dGF0ZWQFCGRvd25mYWxsAAAAPwULdGVtcGVyYXR1cmXNzEw/AAoFdGFpZ2EFCGRvd25mYWxszcxMPwULdGVtcGVyYXR1cmUAAIA+AAoLdGFpZ2FfaGlsbHMFCGRvd25mYWxszcxMPwULdGVtcGVyYXR1cmUAAIA+AAoNdGFpZ2FfbXV0YXRlZAUIZG93bmZhbGzNzEw/BQt0ZW1wZXJhdHVyZQAAgD4ACgd0aGVfZW5kBQhkb3duZmFsbAAAAD8FC3RlbXBlcmF0dXJlAAAAPwAKCndhcm1fb2NlYW4FCGRvd25mYWxsAAAAPwULdGVtcGVyYXR1cmUAAAA/AAA="

typedef struct {
	char *identity;
	char *client;
} misc_login_tokens_t;

typedef struct {
	char *uuid;
	char *version;
	uint64_t size;
	char *content_key;
	char *sub_pack_name;
	char *content_identity;
	char has_scripts;
} misc_behavior_pack_info_t;

typedef struct {
	misc_behavior_pack_info_t *infos;
	int16_t size;
} misc_behavior_pack_infos_t;

typedef struct {
	char *uuid;
	char *version;
	uint64_t size;
	char *content_key;
	char *sub_pack_name;
	char *content_identity;
	char has_scripts;
	char rtx_enabled;
} misc_texture_pack_info_t;

typedef struct {
	misc_texture_pack_info_t *infos;
	int16_t size;
} misc_texture_pack_infos_t;

typedef struct {
	char *uuid;
	char *version;
	char *name;
} misc_resource_pack_id_version_t;

typedef struct {
	misc_resource_pack_id_version_t *id_versions;
	uint32_t size;
} misc_resource_pack_id_versions_t;

typedef struct {
	char **ids;
	int16_t size;
} misc_resource_pack_ids_t;

typedef struct {
	char *name;
	char enabled;
} misc_experiment_t;

typedef struct {
	misc_experiment_t *entries;
	int32_t size;
} misc_experiments_t;

typedef union {
	char boolean;
	int32_t signed_var_int;
	float float_le;
} misc_multi_game_rule_t;

typedef struct {
	char *name;
	char editable;
	uint32_t type;
	misc_multi_game_rule_t value;
} misc_game_rule_t;

typedef struct {
	misc_game_rule_t *entries;
	uint32_t size;
} misc_game_rules_t;

typedef struct {
	char *button_name;
	char *link_uri;
} misc_education_shared_resource_uri_t;

typedef struct {
	char *name;
	int16_t runtime_id;
	char component_based;
} misc_item_state_t;

typedef struct {
	misc_item_state_t *entries;
	uint32_t size;
} misc_item_states_t;

typedef struct {
	char *name;
	binary_stream_t *nbt_stream;
} misc_block_property_t;

typedef struct {
	misc_block_property_t *entries;
	uint32_t size;
} misc_block_properties_t;

typedef struct {
	uint16_t with_nbt;
	char nbt_version;
	binary_stream_t *nbt_stream;
	int32_t can_place_on_size;
	binary_stream_t *can_place_on;
	int32_t can_destroy_size;
	binary_stream_t *can_destroy;
	int64_t blocking_tick;
} misc_item_extra_data_t;

typedef struct {
	int32_t network_id;
	uint16_t count;
	uint32_t metadata;
	char has_stack_id;
	int32_t stack_id;
	int32_t block_runtime_id;
	misc_item_extra_data_t extra;
} misc_item_t;

char *get_misc_string_var_int(binary_stream_t *stream);

char *get_misc_string_int_le(binary_stream_t *stream);

binary_stream_t get_misc_byte_array_var_int(binary_stream_t *stream);

binary_stream_t get_misc_byte_array_signed_var_int(binary_stream_t *stream);

binary_stream_t get_misc_byte_array_short_le(binary_stream_t *stream);

misc_login_tokens_t get_misc_login_tokens(binary_stream_t *stream);

misc_behavior_pack_infos_t get_misc_behavior_pack_infos(binary_stream_t *stream);

misc_texture_pack_infos_t get_misc_texture_pack_infos(binary_stream_t *stream);

misc_resource_pack_id_versions_t get_misc_resource_pack_id_versions(binary_stream_t *stream);

misc_resource_pack_ids_t get_misc_resource_pack_ids(binary_stream_t *stream);

misc_experiment_t get_misc_experiment(binary_stream_t *stream);

misc_experiments_t get_misc_experiments(binary_stream_t *stream);

misc_game_rule_t get_misc_game_rule(binary_stream_t *stream);

misc_game_rules_t get_misc_game_rules(binary_stream_t *stream);

misc_education_shared_resource_uri_t get_misc_education_shared_resource_uri(binary_stream_t *stream);

void put_misc_string_var_int(char *value, binary_stream_t *stream);

void put_misc_string_int_le(char *value, binary_stream_t *stream);

void put_misc_byte_array_var_int(binary_stream_t value, binary_stream_t *stream);

void put_misc_byte_array_signed_var_int(binary_stream_t value, binary_stream_t *stream);

void put_misc_byte_array_short_le(binary_stream_t value, binary_stream_t *stream);

void put_misc_login_tokens(misc_login_tokens_t value, binary_stream_t *stream);

void put_misc_behavior_pack_infos(misc_behavior_pack_infos_t value, binary_stream_t *stream);

void put_misc_texture_pack_infos(misc_texture_pack_infos_t value, binary_stream_t *stream);

void put_misc_resource_pack_id_versions(misc_resource_pack_id_versions_t value, binary_stream_t *stream);

void put_misc_resource_pack_ids(misc_resource_pack_ids_t value, binary_stream_t *stream);

void put_misc_experiment(misc_experiment_t value, binary_stream_t *stream);

void put_misc_experiments(misc_experiments_t value, binary_stream_t *stream);

void put_misc_game_rule(misc_game_rule_t value, binary_stream_t *stream);

void put_misc_game_rules(misc_game_rules_t value, binary_stream_t *stream);

void put_misc_education_shared_resource_uri(misc_education_shared_resource_uri_t value, binary_stream_t *stream);

#endif