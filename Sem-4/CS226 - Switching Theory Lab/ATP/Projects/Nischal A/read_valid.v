module read_valid(valid);
output valid;
reg valid;
integer data_file    ; // file handler
integer scan_file    ; // file handler
`define NULL 0    

//File empty - access denied.
initial begin
  data_file = $fopen("access_grant.dat", "r");
  if (data_file == `NULL) begin
	valid = 0;
  end
end

initial begin
scan_file = $fscanf(data_file, "%d\n", valid);
  $display("valid is %d", valid);
  if (!$feof(data_file)) begin
    $display("valid is %d", valid);
  end
end
endmodule
