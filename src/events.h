#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>

struct tofi;

void event_open(struct tofi *tofi);
void event_input(struct tofi *tofi, const char *input, size_t result_count);
void event_select(struct tofi *tofi, uint32_t index, const char *value);
void event_submit(struct tofi *tofi, uint32_t index, const char *value);
void event_close(struct tofi *tofi, const char *reason);

#endif /* EVENTS_H */
