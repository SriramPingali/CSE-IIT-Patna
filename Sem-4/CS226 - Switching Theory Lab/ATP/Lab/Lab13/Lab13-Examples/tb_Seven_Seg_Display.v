
module tb_Seven_Seg_Display(); 
wire [6: 0] Display; 
reg [6: 0] BCD;

Seven_Seg_Display DUT(Display, BCD);
initial begin
#5 BCD = 4'b0000;
#5 BCD = 4'b0001;
#5 BCD = 4'b0010;
#5 BCD = 4'b0011;
#5 BCD = 4'b0100;
#5 BCD = 4'b0101;
#5 BCD = 4'b0110;
#5 BCD = 4'b0111;
#5 BCD = 4'b1000;
#5 BCD = 4'b1001;
#5 BCD = 4'b1010;
#5 BCD = 4'b1011;
#5 BCD = 4'b1100;
#5 BCD = 4'b1101;
#5 BCD = 4'b1110;
#5 BCD = 4'b1110;
end 
endmodule




































 
















