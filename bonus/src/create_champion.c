/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** create_champion.c
*/

#include "corewar.h"
#include <fcntl.h>

static int open_champion_file(const char *filename)
{
    int fd = open(filename, O_RDONLY);

    if (fd == -1)
        return -1;
    return fd;
}

static int read_champion_header(int fd, header_t *header)
{
    ssize_t read_size = read(fd, header, sizeof(header_t));
    int prog_size;

    if (read_size == -1 || read_size != sizeof(header_t))
        return error_msg("Error: invalid champion file.\n", 84);
    if (swap_endian(header->magic) != COREWAR_EXEC_MAGIC)
        return error_msg("Error: invalid magic number.\n", 84);
    prog_size = swap_endian(header->prog_size);
    if (prog_size <= 0 || prog_size > CHAMP_MAX_SIZE)
        return error_msg("Error: invalid program size.\n", 84);
    return prog_size;
}

static void fill_champion_info(champion_t *champ, header_t *header,
    int prog_size)
{
    champ->size = prog_size;
    my_strncpy(champ->name, header->prog_name, PROG_NAME_LENGTH);
    champ->name[PROG_NAME_LENGTH] = '\0';
    my_strncpy(champ->comment, header->comment, COMMENT_LENGTH);
    champ->comment[COMMENT_LENGTH] = '\0';
}

static int read_champion_code(int fd, champion_t *champ)
{
    champ->code = malloc(champ->size);
    if (!champ->code)
        return error_msg("Error: memory allocation failed.\n", 84);
    if (read(fd, champ->code, champ->size) != champ->size) {
        free(champ->code);
        champ->code = NULL;
        return error_msg("Error: invalid champion file.\n", 84);
    }
    return 0;
}

int read_champion_file(champion_t *champ)
{
    int fd = open_champion_file(champ->filename);
    header_t header;
    int prog_size;

    if (fd == -1)
        return error_msg("Error: cannot open champion file.\n", 84);
    prog_size = read_champion_header(fd, &header);
    if (prog_size == 84) {
        close(fd);
        return 84;
    }
    fill_champion_info(champ, &header, prog_size);
    if (read_champion_code(fd, champ) == 84) {
        close(fd);
        return 84;
    }
    close(fd);
    return 0;
}
