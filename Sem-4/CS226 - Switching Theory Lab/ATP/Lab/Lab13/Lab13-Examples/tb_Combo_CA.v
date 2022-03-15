

module tb_Combo_CA();
reg A, B, C, D;
wire Y;
Combo_CA M0 (Y, A, B, C, D);
initial begin
#5 {A, B, C, D} = 4'b0000;
#5 {A, B, C, D} = 4'b0001;
#5 {A, B, C, D} = 4'b0010;
#5 {A, B, C, D} = 4'b0011;
#5 {A, B, C, D} = 4'b0100;
#5 {A, B, C, D} = 4'b0101;
#5 {A, B, C, D} = 4'b0110;
#5 {A, B, C, D} = 4'b0111;
#5 {A, B, C, D} = 4'b1000;
#5 {A, B, C, D} = 4'b1001;
#5 {A, B, C, D} = 4'b1010;
#5 {A, B, C, D} = 4'b1011;
#5 {A, B, C, D} = 4'b1100;
#5 {A, B, C, D} = 4'b1101;
#5 {A, B, C, D} = 4'b1110;
#5 {A, B, C, D} = 4'b1111;
end
initial begin #500 $finish; end
//initial begin $monitor ($time,"%h %b", {A, B, C, D}, Y); end
endmodule