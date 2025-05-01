#ifndef EXAMPLE_LIBRARY_H
#define EXAMPLE_LIBRARY_H

#include "l_system.h"

L_System example_library[10] = {
    [0] = {
        .axiom = "X",
        .rules_for_indices = {0},
        .rules = {
            [0] = {.character = 'X', .rule = "F[+X][-X]FX"},
            [1] = {.character = 'F', .rule = "FF"}
        },
        .iterations = 10,
        .turn_angle = 45.0f,
        .start_direction = 0.0f
    },
    [1] = {
        .axiom = "-X",
        .rules_for_indices = {0, 1},
        .rules = {
            [0] = {.character = 'X', .rule = "F-[[X]+X]+F[+FX]-X"},
            [1] = {.character = 'F', .rule = "FF"}
        },
        .iterations = 8,
        .turn_angle = 25.0f,
        .start_direction = 90.0f
    },
    [2] = {
        .axiom = "Y",
        .rules_for_indices = {0, 1},
        .rules = {
            [0] = {.character = 'X', .rule = "X[-FFF][+FFF]FX"},
            [1] = {.character = 'Y', .rule = "YFX[+Y][-Y]"}
        },
        .iterations = 10,
        .turn_angle = 25.7f,
        .start_direction = 90.0f
    },
    [3] = {
        .axiom = "F",
        .rules_for_indices = {0},
        .rules = {
            [0] = {.character = 'F', .rule = "FF+[+F-F-F]-[-F+F+F]"}
        },
        .iterations = 6,
        .turn_angle = 22.5f,
        .start_direction = 90.0f
    },
    [4] = {
        .axiom = "VZFFF",
        .rules_for_indices = {0, 1, 2, 3, 4},
        .rules = {
            [0] = {.character = 'V', .rule = "[+++W][---W]YV"},
            [1] = {.character = 'W', .rule = "+X[-W]Z"},
            [2] = {.character = 'X', .rule = "-W[+X]Z"},
            [3] = {.character = 'Y', .rule = "YZ"},
            [4] = {.character = 'Z', .rule = "[-FFF][+FFF]F"}
        },
        .iterations = 14,
        .turn_angle = 20.0f,
        .start_direction = 90.0f
    },
    [5] = {
        .axiom = "F+F+F+F",
        .rules_for_indices = {0},
        .rules = {
            [0] = {.character = 'F', .rule = "FF+F+F+F+FF"}
        },
        .iterations = 6,
        .turn_angle = 90.0f,
        .start_direction = 0.0f
    },
    [6] = {
        .axiom = "YF",
        .rules_for_indices = {0, 1},
        .rules = {
            [0] = {.character = 'X', .rule = "YF+XF+Y"},
            [1] = {.character = 'Y', .rule = "XF-YF-X"}
        },
        .iterations = 11,
        .turn_angle = 60.0f,
        .start_direction = 180.0f
    },
    [7] = {
        .axiom = "F++F++F++F++F",
        .rules_for_indices = {0},
        .rules = {
            [0] = {.character = 'F', .rule = "F++F++F+++++F-F++F"}
        },
        .iterations = 6,
        .turn_angle = 36.0f,
        .start_direction = 0.0f
    },
    [8] = {
        .axiom = "FX",
        .rules_for_indices = {0, 1},
        .rules = {
            [0] = {.character = 'X', .rule = "X+YF+"},
            [1] = {.character = 'Y', .rule = "-FX-Y"}
        },
        .iterations = 18,
        .turn_angle = 90.0f,
        .start_direction = 0.0f
    },
    [9] = {
        .axiom = "XF",
        .rules_for_indices = {0, 1},
        .rules = {
            [0] = {.character = 'X', .rule = "X+YF++YF-FX--FXFX-YF+"},
            [1] = {.character = 'Y', .rule = "-FX+YFYF++YF+FX--FX-Y"}
        },
        .iterations = 5,
        .turn_angle = 60.0f,
        .start_direction = 0.0f
    }
};

#endif