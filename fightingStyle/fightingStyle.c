#include "fightingStyle.h"

StyleStats get_style_stats(FightingStyle style) {
    switch (style) {
        case STYLE_BOXING:
            return (StyleStats){5, 7, 1, 1};

        case STYLE_MUAY_THAI:
            return (StyleStats){5, 6, 2, 1};

        case STYLE_TAEKWONDO:
            return (StyleStats){3, 9, 0, 3};

        case STYLE_BJJ:
            return (StyleStats){8, 4, 2, 0};

        case STYLE_KARATE:
            return (StyleStats){5, 7, 1, 2};

        default:
            return (StyleStats){5, 5, 0, 0};
    }
}
