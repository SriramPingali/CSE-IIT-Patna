; outline of bubble sort program
        ldc 0x1000
        a2sp
        adj -1
        ldc   10
        stl   0
        ldc   array
        call  sort
        HALT
;
; Areg= return address
; Breg= address of elements
; SP+0= number of elements
sort: 
;ok, you're on your own now
      return

array:  data    -559038737
	data    1592639189
      	data    -1058161986
	data    1588527317
        data    -1420028582
	data    -899895827
	data    -17958194
	data    -1061097459
	data    -556882451
	data    1358610133

