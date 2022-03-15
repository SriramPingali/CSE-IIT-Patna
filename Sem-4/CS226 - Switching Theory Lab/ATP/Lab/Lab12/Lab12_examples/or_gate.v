// name: or_gate
 
module or_gate(out,a,b);  //you list all inputs and outputs, by convention outputs go first
output out;                          // this tells the compile which lines are inputs and outputs 
input a, b;

assign out = a | b;         // output function
endmodule

