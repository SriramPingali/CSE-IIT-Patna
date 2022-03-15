// Binary to Gray Code Converter
module p1(gray,binary);
    // Mode Declaration
    input [3:0] binary;     // Binary Form
    output [3:0] gray;      // Gray Form

    assign gray[3] = binary[3];
    assign gray[2] = binary[3] ^ binary[2];
    assign gray[1] = binary[2] ^ binary[1];
    assign gray[0] = binary[1] ^ binary[0];
endmodule