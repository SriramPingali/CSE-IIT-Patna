// Testbench for module (a) in p5.v
module tb_p5_vat_buzzer_behavior();
    // Mode Declaration
    reg above_25_0, above_30_0, low_level_0,above_25_1, above_30_1, low_level_1,select_vat_1;
    wire buzzer;
    
    // Instantiate Module
    vat_buzzer_behavior UUT (buzzer,above_25_0,above_30_0,low_level_0,above_25_1,above_30_1,low_level_1,select_vat_1);

    initial begin
        select_vat_1 = 0;
        low_level_0 = 0;
        above_30_0 = 0;
        above_25_0 = 0;
        low_level_1 = 0;
        above_30_1 = 0;
        above_25_1 = 0;
        
        // Testcase: All possible combinations of input parameters
        repeat(127) begin
            #10;
            select_vat_1 = select_vat_1 ^ (low_level_0 & above_30_0 & above_25_0 & low_level_1 & above_30_1 & above_25_1);
            low_level_0 = low_level_0 ^ (above_30_0 & above_25_0 & low_level_1 & above_30_1 & above_25_1);
            above_30_0 = above_30_0 ^ (above_25_0 & low_level_1 & above_30_1 & above_25_1);
            above_25_0 = above_25_0 ^ (low_level_1 & above_30_1 & above_25_1);
            low_level_1 = low_level_1 ^ (above_30_1 & above_25_1);
            above_30_1 = above_30_1 ^ above_25_1;
            above_25_1 = above_25_1 ^ 1;
        end
    end

    initial begin
        $display("select_vat_1  low_level_0  above_30_0  above_25_0  low_level_1  above_30_1  above_25_1  buzzer");
        $monitor("    %b            %b           %b          %b          %b           %b           %b        %b",select_vat_1,low_level_0,above_30_0,above_25_0,low_level_1,above_30_1,above_25_1,buzzer);
    end
endmodule

// Testbench for module (b) in p5.v
module tb_p5_alarm_priority();
    // Mode Declaration
    wire [2:0] intruder_zone;
    wire valid;
    reg [1:8] zone;

    // Instantiate Module
    alarm_priority DUT (intruder_zone,valid,zone);

    // Testcase: All possible combinations of input parameters
    initial begin
        zone = 8'b0;
        repeat(127) begin
            #10 zone = zone+1;
        end
    end

    initial begin
        $display("zone intruder_zone valid");
        $monitor(" %d       %d         %d",zone,intruder_zone,valid);
    end
endmodule