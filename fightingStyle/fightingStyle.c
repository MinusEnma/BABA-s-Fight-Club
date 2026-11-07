#include "fightingStyle.h"

StyleStats get_style_stats(FightingStyle style) {
    switch (style) {
        case STYLE_BOXING:
            return (StyleStats){5, 3, 0, 0};

        case STYLE_MUAY_THAI:
            return (StyleStats){5, 3, 0, 0};

        case STYLE_TAEKWONDO:
            return (StyleStats){3, 4, 0, 0};

        case STYLE_BJJ:
            return (StyleStats){8, 2, 0, 0};

        case STYLE_KARATE:
            return (StyleStats){5, 3, 0, 0};

        default:
            return (StyleStats){5, 2, 0, 0};
    }
}
