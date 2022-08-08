/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import org.apache.hadoop.io.Writable;
import org.apache.hadoop.io.WritableUtils;


public class FraudWritable implements Writable{

    private String custName;
    private String recieveDate;
    private boolean returned;
    private String returnDate;
    
    public FraudWritable()
    {
	set("","","no","");
    }

    void set(String custName, String recieveDate, String returned, String returnDate) {
        this.custName=custName;
        this.recieveDate=recieveDate;
        if(returned.equalsIgnoreCase("yes"))
        {
            this.returned=true;
        }
        else
        {
            this.returned=false;
        }
        this.returnDate=returnDate;
    }
    String getCustomerName()
    {
        return this.custName;
    }
    String getrecieveDate()
    {
        return this.recieveDate;
    }
    boolean getreturn()
    {
        return this.returned;
    }
    String getreturnDate()
    {
        return this.returnDate;
    }
    @Override
    public void write(DataOutput d) throws IOException {
        WritableUtils.writeString(d, this.custName);
        WritableUtils.writeString(d, this.recieveDate);
        d.writeBoolean(this.returned);
        WritableUtils.writeString(d, this.returnDate); 
    }

    @Override
    public void readFields(DataInput di) throws IOException {
        this.custName=WritableUtils.readString(di);
        this.recieveDate=WritableUtils.readString(di);
        this.returned=di.readBoolean();
        this.returnDate=WritableUtils.readString(di);
    }
}
