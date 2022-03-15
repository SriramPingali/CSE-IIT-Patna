
module tb_ALU();
  wire[7:0] Result;
  reg [7:0] A,B;
  reg [1:0] opcode;
 
  ALU DUT (Result,opcode,A,B);

  initial 
  begin
  #0A=8'd0; B=8'd0; opcode=2'b00;
  #5A=8'd1; B=8'd3; opcode=2'b01;
  #5A=8'd3; B=8'd4; opcode=2'b10;
  #5A=8'd10; B=8'd10; opcode=2'b11;
  #5A=8'd15; B=8'd1; opcode=2'b00;
  #5A=8'd20; B=8'd20; opcode=2'b10;
  #5A=8'd34; B=8'd5; opcode=2'b10;
  #5A=8'd255; B=8'd0; opcode=2'b11;
  #5A=8'd15; B=8'd1; opcode=2'b00;
  #5A=8'd200; B=8'd61; opcode=2'b00;
  end
  endmodule


