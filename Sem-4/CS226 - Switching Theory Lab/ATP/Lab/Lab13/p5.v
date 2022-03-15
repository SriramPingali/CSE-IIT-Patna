// Module (a)
module vat_buzzer_behavior ( output buzzer,
    input above_25_0, above_30_0, low_level_0,
    input above_25_1, above_30_1, low_level_1,
    input select_vat_1 );

    assign buzzer = select_vat_1 ? (low_level_1 | (above_30_1 | ~above_25_1))
                    : (low_level_0 | (above_30_0 | ~above_25_0));
endmodule


// Module (b)
module alarm_priority ( output [2:0] intruder_zone,
                        output valid,
                        input [1:8] zone );
    wire [1:8] winner;

    assign winner[1] = zone[1];
    assign winner[2] = zone[2] & ~zone[1];
    assign winner[3] = zone[3] & ~(zone[2] | zone[1]);
    assign winner[4] = zone[4] & ~(zone[3] | zone[2] | zone[1]);
    assign winner[5] = zone[5] & ~(zone[4] | zone[3] | zone[2] |
    zone[1]);
    assign winner[6] = zone[6] & ~(zone[5] | zone[4] | zone[3] |
    zone[2] | zone[1]);
    assign winner[7] = zone[7] & ~(zone[6] | zone[5] | zone[4] |
    zone[3] | zone[2] | zone[1]);
    assign winner[8] = zone[8] & ~(zone[7] | zone[6] | zone[5] |
    zone[4] | zone[3] | zone[2] |
    zone[1]);
    assign intruder_zone[2] = winner[5] | winner[6] |
    winner[7] | winner[8];
    assign intruder_zone[1] = winner[3] | winner[4] |
    winner[7] | winner[8];
    assign intruder_zone[0] = winner[2] | winner[4] |
    winner[6] | winner[8];
    
    assign valid = zone[1] | zone[2] | zone[3] | zone[4] |
    zone[5] | zone[6] | zone[7] | zone[8];
endmodule