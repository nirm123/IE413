

         MODEL DEFAULTS
         --------------

Model Name:         HW5
Model Description:  
Output File:        HW5.out
Output Plot Style:  NOAUTO_FIT
Run Mode:           SINGLE_STEP
Trace Vars:         POISSON[2],VARS[4],WAIT[VARS[5]],VARS[1]
Random Number Seed: 12345
Initial Values:     
Ending Condition:   STOP_ON_TIME
Ending Time:        1000.000
Trace Events:       ENTER,EXIT,LEAVE
Hide Edges:         



         STATE VARIABLES
         ---------------

     State Variable #1
Name:          POISSON
Description:   
Type:          REAL
Size:          3

     State Variable #2
Name:          GROUPS
Description:   
Type:          INTEGER
Size:          1000

     State Variable #3
Name:          WAIT
Description:   
Type:          REAL
Size:          1000

     State Variable #4
Name:          VARS
Description:   
Type:          INTEGER
Size:          8



          VERTICES
          --------

     Vertex #1
Name:             INIT
Description:      
State Changes:    VARS[0]=-1,VARS[6]=28,VARS[7]=0
Parameter(s):     
Bitmap(Inactive): 
Bitmap(Active):   
Use Flowchart Shapes:   0
Use Opaque Bitmaps:   0
Location:         X:  0.10;  Y:  2.49;  Z: -1.00;
Local Trace:      
Trace Location:   Bottom

     Vertex #2
Name:             ARR
Description:      
State Changes:    POISSON[0]=0.03019738342,POISSON[1]=1,POISSON[2]=0,VARS[0]=VARS[0]+1,WAIT[999]=-10*LN{1-RND}
Parameter(s):     
Bitmap(Inactive): 
Bitmap(Active):   
Use Flowchart Shapes:   0
Use Opaque Bitmaps:   0
Location:         X:  0.10;  Y:  1.93;  Z: -1.00;
Local Trace:      
Trace Location:   Bottom

     Vertex #3
Name:             NUM
Description:      
State Changes:    POISSON[1]=RND*POISSON[1],POISSON[2]=POISSON[2]+(POISSON[0]<=POISSON[1]),GROUPS[VARS[0]]=POISSON[2]*(POISSON[2]!=0)+-1*(POISSON[2]==0),WAIT[VARS[0]]=CLK
Parameter(s):     
Bitmap(Inactive): 
Bitmap(Active):   
Use Flowchart Shapes:   0
Use Opaque Bitmaps:   0
Location:         X: -0.67;  Y:  1.93;  Z: -1.00;
Local Trace:      
Trace Location:   Bottom

     Vertex #4
Name:             START
Description:      
State Changes:    VARS[1]=VARS[6],VARS[2]=0
Parameter(s):     
Bitmap(Inactive): 
Bitmap(Active):   
Use Flowchart Shapes:   0
Use Opaque Bitmaps:   0
Location:         X:  0.96;  Y:  2.49;  Z: -1.00;
Local Trace:      
Trace Location:   Bottom

     Vertex #5
Name:             SELECT
Description:      
State Changes:    VARS[3]=(GROUPS[VARS[2]]<VARS[1])&(GROUPS[VARS[2]]!=-1),VARS[1]=VARS[1]-GROUPS[VARS[2]]*VARS[3],VARS[4]=GROUPS[VARS[2]],GROUPS[VARS[2]]=GROUPS[VARS[2]]-(GROUPS[VARS[2]]+1)*VARS[3],VARS[5]=VARS[2],VARS[2]=VARS[2]+1
Parameter(s):     
Bitmap(Inactive): 
Bitmap(Active):   
Use Flowchart Shapes:   0
Use Opaque Bitmaps:   0
Location:         X:  1.78;  Y:  2.49;  Z: -1.00;
Local Trace:      
Trace Location:   Bottom

     Vertex #6
Name:             ENTER
Description:      
State Changes:    
Parameter(s):     
Bitmap(Inactive): 
Bitmap(Active):   
Use Flowchart Shapes:   0
Use Opaque Bitmaps:   0
Location:         X: -0.67;  Y:  2.39;  Z: -1.00;
Local Trace:      
Trace Location:   Bottom

     Vertex #7
Name:             EXIT
Description:      
State Changes:    WAIT[VARS[5]]=CLK-WAIT[VARS[5]]
Parameter(s):     
Bitmap(Inactive): 
Bitmap(Active):   
Use Flowchart Shapes:   0
Use Opaque Bitmaps:   0
Location:         X:  2.46;  Y:  2.48;  Z: -1.00;
Local Trace:      
Trace Location:   Bottom

     Vertex #8
Name:             LEAVE
Description:      
State Changes:    
Parameter(s):     
Bitmap(Inactive): 
Bitmap(Active):   
Use Flowchart Shapes:   0
Use Opaque Bitmaps:   0
Location:         X:  1.78;  Y:  2.88;  Z: -1.00;
Local Trace:      
Trace Location:   Bottom

     Vertex #9
Name:             CLEAR
Description:      
State Changes:    GROUPS[VARS[7]]=0,WAIT[VARS[7]]=0,VARS[7]=VARS[7]+1
Parameter(s):     
Bitmap(Inactive): 
Bitmap(Active):   
Use Flowchart Shapes:   0
Use Opaque Bitmaps:   0
Location:         X:  0.10;  Y:  2.85;  Z: -1.00;
Local Trace:      
Trace Location:   Bottom



          EDGES
          -----


     Graphics Edge #1

  Sub-Edge #1
Description:   
Type:          Scheduling
Origin:        INIT
Destination:   ARR
Condition:     1==1
Delay:         -10*LN{1-RND}
Priority:      5
Attributes:    

     Graphics Edge #2

  Sub-Edge #2
Description:   
Type:          Scheduling
Origin:        ARR
Destination:   ARR
Condition:     (CLK+WAIT[999])<510
Delay:         WAIT[999]
Priority:      5
Attributes:    

     Graphics Edge #3

  Sub-Edge #3
Description:   
Type:          Scheduling
Origin:        ARR
Destination:   NUM
Condition:     1==1
Delay:         0
Priority:      5
Attributes:    

     Graphics Edge #4

  Sub-Edge #4
Description:   
Type:          Scheduling
Origin:        NUM
Destination:   NUM
Condition:     POISSON[0]<=POISSON[1]
Delay:         0
Priority:      5
Attributes:    

     Graphics Edge #5

  Sub-Edge #5
Description:   
Type:          Scheduling
Origin:        INIT
Destination:   START
Condition:     1==1
Delay:         60
Priority:      5
Attributes:    

     Graphics Edge #6

  Sub-Edge #6
Description:   
Type:          Scheduling
Origin:        START
Destination:   START
Condition:     CLK<600
Delay:         60
Priority:      5
Attributes:    

     Graphics Edge #7

  Sub-Edge #7
Description:   
Type:          Scheduling
Origin:        START
Destination:   SELECT
Condition:     1==1
Delay:         0
Priority:      5
Attributes:    

     Graphics Edge #8

  Sub-Edge #8
Description:   
Type:          Scheduling
Origin:        SELECT
Destination:   SELECT
Condition:     (VARS[1]>0)&(GROUPS[VARS[2]]!=0)
Delay:         0
Priority:      5
Attributes:    

     Graphics Edge #9

  Sub-Edge #9
Description:   
Type:          Scheduling
Origin:        NUM
Destination:   ENTER
Condition:     POISSON[0]>POISSON[1]
Delay:         0
Priority:      5
Attributes:    

     Graphics Edge #10

  Sub-Edge #10
Description:   
Type:          Scheduling
Origin:        SELECT
Destination:   EXIT
Condition:     VARS[3]
Delay:         0
Priority:      5
Attributes:    

     Graphics Edge #11

  Sub-Edge #11
Description:   
Type:          Scheduling
Origin:        SELECT
Destination:   LEAVE
Condition:     (VARS[1]==0)|(GROUPS[VARS[2]]==0)
Delay:         0
Priority:      5
Attributes:    

     Graphics Edge #12

  Sub-Edge #12
Description:   
Type:          Scheduling
Origin:        INIT
Destination:   CLEAR
Condition:     1==1
Delay:         0
Priority:      5
Attributes:    

     Graphics Edge #13

  Sub-Edge #13
Description:   
Type:          Scheduling
Origin:        CLEAR
Destination:   CLEAR
Condition:     VARS[7]<1000
Delay:         0
Priority:      5
Attributes:    

