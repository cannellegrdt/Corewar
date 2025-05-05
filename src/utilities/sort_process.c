/*
** EPITECH PROJECT, 2024
** B-CPE-200-LYN-2-1-corewar-cannelle.gourdet
** File description:
** sort_process.c
*/

#include "corewar.h"

static process_t *find_predecessor(process_t *head, process_t *process)
{
    process_t *pred = head;

    while (pred && pred->next != process)
        pred = pred->next;
    return pred;
}

static process_t *swap_adjacent_processes(vm_t *vm, process_t *process,
    process_t *pred)
{
    process_t *temp = process->next;

    process->next = temp->next;
    temp->next = process;
    if (process == vm->processes)
        vm->processes = temp;
    else
        pred->next = temp;
    return temp;
}

static bool is_sorting_needed(vm_t *vm)
{
    return (vm && vm->processes && vm->process_count > 1);
}

void sort_processes_by_champion_number(vm_t *vm)
{
    bool swapped;
    process_t *ptr1;
    process_t *lptr = NULL;
    process_t *pred = NULL;

    if (!is_sorting_needed(vm))
        return;
    do {
        swapped = false;
        ptr1 = vm->processes;
        while (ptr1->next != lptr) {
            pred = (ptr1 != vm->processes) ?
            find_predecessor(vm->processes, ptr1) : NULL;
            if (ptr1->champion_number > ptr1->next->champion_number)
                swapped = true;
            ptr1 = (ptr1->champion_number > ptr1->next->champion_number) ?
            swap_adjacent_processes(vm, ptr1, pred) : ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}
