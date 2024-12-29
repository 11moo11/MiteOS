#ifndef CSV_H_INCLUDED
#define CSV_H_INCLUDED

class CSV {
    public:
        static String getElement(String csv, uint8_t index) {
            uint16_t tindex = 0;
            for(uint8_t i = index; i > 0; i--) {
                tindex = csv.indexOf(',', tindex) + 1;
            }
            return csv.substring(tindex, csv.indexOf(',', tindex));
        }
        
        static uint8_t getLength(String csv) {
            uint8_t item_count = 1;
            for (int i = 0; i < csv.length(); i++) {
                if (csv.charAt(i) == ',') {
                    item_count++;
                }
            }
            return item_count;
        }
};

#endif