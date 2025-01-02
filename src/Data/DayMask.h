#ifndef DAY_MASK_H
#define DAY_MASK_H

struct DayMask {
	bool monday : 1;
	bool tuesday : 1;
	bool wednesday : 1;
	bool thursday : 1;
	bool friday : 1;
	bool saturday : 1;
	bool sunday : 1;
	bool once : 1;
};

static uint8_t packDayMask(const DayMask& mask) {
    uint8_t packed = 0;
    packed |= (mask.monday << 0);
    packed |= (mask.tuesday << 1);
    packed |= (mask.wednesday << 2);
    packed |= (mask.thursday << 3);
    packed |= (mask.friday << 4);
    packed |= (mask.saturday << 5);
    packed |= (mask.sunday << 6);
    packed |= (mask.once << 7);
    return packed;
}

static DayMask unpackDayMask(uint8_t packed) {
    DayMask mask;
    mask.monday = (packed >> 0) & 1;
    mask.tuesday = (packed >> 1) & 1;
    mask.wednesday = (packed >> 2) & 1;
    mask.thursday = (packed >> 3) & 1;
    mask.friday = (packed >> 4) & 1;
    mask.saturday = (packed >> 5) & 1;
    mask.sunday = (packed >> 6) & 1;
    mask.once = (packed >> 7) & 1;
    return mask;
}
#endif