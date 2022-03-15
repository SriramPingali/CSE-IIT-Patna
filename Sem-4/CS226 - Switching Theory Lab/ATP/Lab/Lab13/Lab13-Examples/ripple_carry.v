
module ripple_carry(s,cout,cin,A,B);
input [3:0] A, B; // Declaration of two four-bit inputs
input cin; // and the one-bit input carry
output [3:0] s; // Declaration of the five-bit outputs
output cout;
wire [2:0] c;  // internal carry wires
fulladder fa0(s[0],c[0],A[0],B[0],cin );
fulladder fa1(s[1],c[1],A[1],B[1],c[0],);
fulladder fa2(s[2],c[2],A[2],B[2],c[1],);
fulladder fa3(s[3],cout,A[3],B[3],c[2],);

endmodule


module fulladder(sum,cout,a,b,cin,);
  input a,b,cin;
  output sum,cout;
  reg sum,cout;
  always @ (a or b or cin)
  begin
    sum <= a ^ b ^ cin;
    cout <= (a & b) | (a & cin) | (b & cin);
  end
endmodule
