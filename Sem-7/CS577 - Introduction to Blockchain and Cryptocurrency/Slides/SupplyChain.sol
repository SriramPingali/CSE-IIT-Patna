pragma solidity ^0.4.11;

contract supply_Chain {
    uint public _p_id =0;
    uint public _u_id =0;
    uint public _t_id=0;
    
    struct track_product {
        uint _product_id;
        uint _owner_id;
        address _product_owner;
        uint _timeStamp;
    }
    mapping(uint => track_product) public tracks;
    
    struct product {
        string _product_name;
        uint _product_cost;
        string _product_specs;
        string _product_review;
        address _product_owner;
        uint _manufacture_date;
    }
    
    mapping(uint => product) public products;
    
    struct participant {
        string _userName;
        string _passWord;
        address _address;
        string _userType;
    }
    mapping(uint => participant) public participants;
    
    function createParticipant(string name ,string pass ,address u_add ,string utype) public returns (uint){
        uint user_id = _u_id++;
        participants[user_id]._userName = name ;
        participants[user_id]._passWord = pass;
        participants[user_id]._address = u_add;
        participants[user_id]._userType = utype;
        return user_id;
    }
    
    function newProduct(uint own_id, string name ,uint p_cost ,string p_specs ,string p_review) returns (uint) {
        if(keccak256(participants[own_id]._userType) == keccak256("Manufacturer")) {
            uint product_id = _p_id++;
           
            
            products[product_id]._product_name = name;
            products[product_id]._product_cost = p_cost;
            products[product_id]._product_specs =p_specs;
            products[product_id]._product_review =p_review;
            products[product_id]._product_owner = participants[own_id]._address;
            products[product_id]._manufacture_date = now;
            
           
            
            return product_id;
        }
        
       return 0;
    }
    function getParticipant(uint p_id) returns (string,address,string) {
        return (participants[p_id]._userName,participants[p_id]._address,participants[p_id]._userType);
    }

    function getProduct_details(uint prod_id) public returns (string,uint,string,string,address,uint){
        return (products[prod_id]._product_name,products[prod_id]._product_cost,products[prod_id]._product_specs,products[prod_id]._product_review,products[prod_id]._product_owner,products[prod_id]._manufacture_date);
    }
    modifier onlyOwner(uint user1_id,uint pid) {
         if(participants[user1_id]._address != products[pid]._product_owner ) throw;
         _;
        
     }
    
function transferOwnership_product(uint user1_id ,uint user2_id, uint prod_id) onlyOwner(user1_id,prod_id) public returns(bool) {
    
        participant  p1 = participants[user1_id];
        participant  p2 = participants[user2_id];
       
        uint track_id = _t_id++;
      
   
        if(keccak256(p1._userType) == keccak256("Manufacturer") && keccak256(p2._userType)==keccak256("Supplier")){
            tracks[track_id]._product_id =prod_id;
            tracks[track_id]._product_owner = p2._address;
            tracks[track_id]._owner_id = user2_id;
            tracks[track_id]._timeStamp = now;
            products[prod_id]._product_owner = p2._address;
            
            return (true);
        }
        if(keccak256(p1._userType) == keccak256("Supplier") && keccak256(p2._userType)==keccak256("Supplier")){
            tracks[track_id]._product_id =prod_id;
            tracks[track_id]._product_owner = p2._address;
            tracks[track_id]._owner_id = user2_id;
            tracks[track_id]._timeStamp = now;
            products[prod_id]._product_owner = p2._address;
            
            return (true);
        }
        
        else if(keccak256(p1._userType) == keccak256("Supplier") && keccak256(p2._userType)==keccak256("Customer")){
            tracks[track_id]._product_id =prod_id;
            tracks[track_id]._product_owner = p2._address;
            tracks[track_id]._owner_id = user2_id;
            tracks[track_id]._timeStamp = now;
            products[prod_id]._product_owner = p2._address;
            
            return (true);
        }
        
        return (false);
    }
  
    function getProduct_trackindex(uint trck_id)  public  returns (uint,uint,address,uint) {
        
        track_product t = tracks[trck_id];
       
         return (t._product_id,t._owner_id,t._product_owner,t._timeStamp);
    }
    
    function userLogin(uint uid ,string uname ,string pass ,string utype) public returns (bool){
        if(keccak256(participants[uid]._userType) == keccak256(utype)) {
            if(keccak256(participants[uid]._userName) == keccak256(uname)) {
                if(keccak256(participants[uid]._passWord)==keccak256(pass)) {
                    return (true);
                }
            }
        }
        
        return (false);
    }

    struct payment {
       uint senderid;
       uint receiverid;
       address senderaddr;
       address rcvraddr;
       uint prod_id;
       string _product_name;
       uint amount;
   }
   mapping(uint => payment) public payments;
   mapping(address => uint256) public balances;

  function PaymentDetail(address _senderaddr, address _rcvraddr,uint _prod_id, string product_name, uint amount) payable public  returns (bool){
        uint _pay_id;
        uint payid = _pay_id++;
         _rcvraddr.call.value(msg.value).gas(20317)();
  }
}