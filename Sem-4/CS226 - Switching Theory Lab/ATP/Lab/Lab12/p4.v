module comparators(eq,neq,lt,lte,gt,gte,a,b);
parameter N = 8;
input [N-1:0] a, b;
output eq, neq;
output lt, lte;
output gt, gte;
assign eq = (a == b);
assign neq = (a != b);
assign lt = (a < b);
assign lte = (a <= b);
assign gt = (a > b);
assign gte = (a >= b);
endmodule
