module tb_Register_File (); 
     wire [31: 0] Data_Out_1, Data_Out_2; 
     reg [31: 0] Data_in; 
     reg [4: 0] Read_Addr_1, Read_Addr_2, Write_Addr; 
     reg Write_Enable, Clock=0; 

     always #5 Clock = ~Clock;

     Register_File DUT(Data_Out_1, Data_Out_2, Data_in, Read_Addr_1, Read_Addr_2, Write_Addr, Write_Enable, Clock); 

     // writing data into the register
     integer i;
     initial begin
          for(i=0; i<32;i=i+1) begin
               #10  Write_Enable=1'b1;Write_Addr=i+1; Data_in=i;
               //#10;
          end 
          #10  Write_Enable=1'b1;Write_Addr=5'b00000; Data_in=32'h1FFF_FFFF;
          #10  Write_Enable=1'b1;Write_Addr=5'b00000; Data_in=32'h0000_0000;
          #10  Write_Enable=1'b1;Write_Addr=5'b00001; Data_in=32'h0000_0001;
          #10  Write_Enable=1'b1;Write_Addr=5'b00010; Data_in=32'h0000_0010;
          #10  Write_Enable=1'b1;Write_Addr=5'b00011; Data_in=32'h0000_0011;
          #10  Write_Enable=1'b1;Write_Addr=5'b00100; Data_in=32'h0000_0100;

          //// read data 
          #10  Write_Enable=1'b1;Read_Addr_1=5'b00100;Read_Addr_2=5'b00001;
          #10  Write_Enable=1'b0;Read_Addr_1=5'b00100;Read_Addr_2=5'b00010;
          #10  Write_Enable=1'b1;Read_Addr_1=5'b00101;Read_Addr_2=5'b00011;
     end
endmodule