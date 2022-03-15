module Arithmetic (A, B, Y1, Y2, Y3, Y4, Y5);

                 input [2:0] A, B;
                 output [3:0] Y1;
                 output [4:0] Y3;
                 output [2:0] Y2, Y4, Y5;
                 reg [3:0] Y1;
                 reg [4:0] Y3;
                 reg [2:0] Y2, Y4, Y5;

                always @(A or B)
                 begin
                                  Y1=A+B;//addition
                                  Y2=A-B;//subtraction
                                  Y3=A*B;//multiplicaton
                                  Y4=A/B;//division
                                  Y5=A%B;//modulus of A divided by B
                 end
endmodule



module Relational (A, B, Y1, Y2, Y3, Y4);
                 input [2:0] A, B;
                 output Y1, Y2, Y3, Y4;
                  reg Y1, Y2, Y3, Y4;

                 always @(A or B)
                 begin
                                  Y1=A<B;//less than
                                  Y2=A<=B;//less than or equal to
                                  Y3=A>B;//greater than
                                  if (A>B)
                                       Y4=1;
                                  else
                                        Y4=0;
                 end
endmodule
 
 
 
 
 module Equality (A, B, Y1, Y2, Y3);
                   input [2:0] A, B;
                   output Y1, Y2;
                   output [2:0] Y3;
                   reg Y1, Y2;
                   reg [2:0] Y3;
 
 		always @(A or B)
                   begin
                                    Y1=A==B;//Y1=1 if A equivalent to B
                                    Y2=A!=B;//Y2=1 if A not equivalent to B
                                    if (A==B)//parenthesis needed
                                         Y3=A;
                                    else
                                         Y3=B;
                   end
 endmodule
 
 
 module Logical (A, B, C, D, E, F, Y);
 
                  input [2:0] A, B, C, D, E, F;
                   output Y;
                   reg Y;
  
                   always @(A or B or C or D or E or F)
                   begin
                                    if ((A==B) && ((C>D) || !(E<F)))
                                          Y=1;
                                   else
                                         Y=0;
                   end
 endmodule
 
 
 module Bitwise (A, B, Y);
 
                 input [6:0] A;
                  input [5:0] B;
                  output [6:0] Y;
                   reg [6:0] Y;
   
                  always @(A or B)
                   begin
                                    Y(0)=A(0)&B(0); //binary AND
                                    Y(1)=A(1)|B(1); //binary OR
                                    Y(2)=!(A(2)&B(2)); //negated AND
                                    Y(3)=!(A(3)|B(3)); //negated OR
                                    Y(4)=A(4)^B(4); //binary XOR
                                    Y(5)=A(5)~^B(5); //binary XNOR
                                    Y(6)=!A(6); //unary negation
                   end
 endmodule
 
 
 module Shift (A, Y1, Y2);
 
                  input [7:0] A;
                   output [7:0] Y1, Y2;
                   parameter B=3; reg [7:0] Y1, Y2;
            
                   always @(A)
 
                  begin
 
                                  Y1=A<<B; //logical shift left
                                    Y2=A>>B; //logical shift right
 
                  end
 endmodule
 

module Concatenation (A, B, Y);

 

                 input [2:0] A, B;
                 output [14:0] Y;
                 parameter C=3'b011;
                 reg [14:0] Y;

                 always @(A or B)
                 begin
                                 Y={A, B, (2{C}}, 3'b110};
                 end
endmodule 


module Reduction (A, Y1, Y2, Y3, Y4, Y5, Y6);

                 input [3:0] A;
                 output Y1, Y2, Y3, Y4, Y5, Y6;
                 reg Y1, Y2, Y3, Y4, Y5, Y6;
                 always @(A)
                 begin
                                  Y1=&A; //reduction AND
                                  Y2=|A; //reduction OR
                                  Y3=~&A; //reduction NAND
                                  Y4=~|A; //reduction NOR
                                  Y5=^A; //reduction XOR
                                  Y6=~^A; //reduction XNOR
                 end
endmodule


module Conditional (Time, Y);
                 input [2:0] Time;
                 output [2:0] Y;
                 reg [2:0] Y;
                 parameter Zero =3b'000;
                 parameter TimeOut = 3b'110;

                 always @(Time)
                 begin
                      Y=(Time!=TimeOut) ? Time +1 : Zero;
                 end

endmodule