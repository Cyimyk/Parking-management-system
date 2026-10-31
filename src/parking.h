
#ifndef PARKING_H
#define PARKING_H

void parking_init(int capacity);
void parking_enter(int car_id);
void parking_leave(int car_id);
void parking_destroy(void);

#endif
