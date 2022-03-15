// Module for Encoder
module p4(op,z1,z2,z3,z4,z5,z6,z7,z8);
    // Mode Declaration
    // Sensor Signal for each zone
    input z1,z2,z3,z4,z5,z6,z7,z8;

    // Output
    output [2:0] op;

    assign op[0] = (z2|z4|z6|z8);
    assign op[1] = (z3|z4|z7|z8);
    assign op[2] = (z5|z6|z7|z8);
endmodule