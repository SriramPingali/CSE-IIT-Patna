# CS561 - ARTIFICIAL INTELLIGENCE LAB
## ASSIGNMENT-5: PROLOG

### Group Name: 1801cs31_1801cs32_1801cs33

| Names           | Roll No. | Batch   |
|-----------------|----------|---------|
| M Maheeth Reddy | 1801CS31 | B.Tech. |
| M Nitesh Reddy  | 1801CS32 | B.Tech. |
| Nischal A       | 1801CS33 | B.Tech. |


### List of files
**Q1_parser.py**:
    Python Code to determine whether an expression is a theorem or not.<br>
    Test Cases:<br>
      - **(P=>Q)=>((~Q=>P)=>Q)** is a Theorem<br>
      - **(P)=>(PvQ)** is a Theorem<br>
      - **(P^Q)=>(PvR)** is a Theorem<br>
      - **(~P)=>(PvQ)** is NOT a Theorem<br>
      - **((P)=>(Q))=>(~PvQ)** is a Theorem<br>

**Q2_prolog.pl**: 
    Prolog Code for representing the given knowledge and answering the given question<br>
    Sample Session:<br>

        Loading: ['Q2_prolog.pl']

        Run: q(X).
        Output: X = b
        

**README.md**:<br>
    - Current file<br>
