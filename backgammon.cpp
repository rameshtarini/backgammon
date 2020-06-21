#include<windows.h> //  header file for gotoxy
#include<iostream>
#include<iomanip>
#include<string>
#include<stdlib.h>
#include<time.h>

COORD coord={0,0}; // this is global variable

using namespace std;

void gotoxy(int x,int y)
{
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

struct location{
    int color=0;
    int numPieces=0;
};

class Piece{
    private:
        int location; //0 thru 28 0 goal for white, 25 goal for red  26&27 for bar
        int color; //red=1, white=2
    public:
        Piece(); //default constructor location = -1 color=0
        void setPiece(int ,int ); //sets pieces at their default location
        void setLocation(int); //sets a piece to a new location
        int checkLocation(int); //returns 1 or 0 if a piece can be moved there
        int checkToGoHome(); //returns 1 or 0 if all pieces are within goal
        int getColor(); //returns color
        int getLocation(); //returns present location
        void display(); //shows what a piece looks like based on color
};

class Board{
    private:
        Piece list[30]; //10 pieces each. 0-9 Player 1, 10-19 Player 2
        location loc[28]; //1-24 on the board (0, 25 are home)
        int status=1; //whos turn? player 1 or 2
        int rbar=0;
        int wbar=0;
        int redpieces=0;
        int whitepieces=0;
        int redhome=0;
        int whitehome=0;
        int redinhome=0;
        int whiteinhome=0;
        int endgame=0;
        int movebarfirst=0;
        int opponent;
        int startcolor;
        int place;
        int startloc;
        int inputdie;
        int position;
        void goFirst();
        void checkBump();
        int pieceAtLoc(int,int); //returns true/false if there is a piece at that location
        void turn(); //involves switching turns
        void move(); //controls all aspects of moving a piece
        int checkbar(int); //checks to see if you have piece(s) on the bar
        int checkmove(int,int); //checks to see if original location to new location is valid
        void validateLoc(int,int); //(from, to) location, subtract/add num pieces and if necessary change color
        void display(); //controls what to print and where on the screen
        void bump(); //sends piece to the bar
        int diceRoll(); //returns the value of one dice roll
    public:
        Board(); //this is fairly long and a lot of hard coding of the position, and using a loop to place them on board
        void execute(); //runs the game
};

Piece::Piece(){
    location=-1;
    color=0;
}

void Piece::setPiece(int l,int c){
    location=l;
    color=c;
}

int Piece::getColor(){
    return color;
}

int Piece::getLocation(){
    return location;
}

void Piece::display(){
    if(getColor()==1)
        cout<<"R";
    if (getColor()==2)
        cout<<"W";
}

//void Piece::setLocation(int); //sets a piece to a new location
//int checkLocation(int); //returns 1 or 0 if a piece can be moved there
//int checkToGoHome(); //returns 1 or 0 if all pieces are within goal

Board::Board(){


    list[0].setPiece(24,1);
    list[1].setPiece(24,1);
    list[2].setPiece(13,1);
    list[3].setPiece(13,1);
    list[4].setPiece(13,1);
    list[5].setPiece(13,1);
    list[6].setPiece(13,1);
    list[7].setPiece(8,1);
    list[8].setPiece(8,1);
    list[9].setPiece(8,1);
    list[10].setPiece(6,1);
    list[11].setPiece(6,1);
    list[12].setPiece(6,1);
    list[13].setPiece(6,1);
    list[14].setPiece(6,1);
    list[15].setPiece(1,2);
    list[16].setPiece(1,2);
    list[17].setPiece(1,2);
    list[18].setPiece(12,2);
    list[19].setPiece(12,2);
    list[20].setPiece(12,2);
    list[21].setPiece(12,2);
    list[22].setPiece(12,2);
    list[23].setPiece(17,2);
    list[24].setPiece(17,2);
    list[25].setPiece(17,2);
    list[26].setPiece(17,2);
    list[27].setPiece(19,2);
    list[28].setPiece(20,2);
    list[29].setPiece(20,2);


    for(int x=0; x<30; x++){
        if(list[x].getColor()==1)
            redpieces++;
        if(list[x].getColor()==2)
            whitepieces++;
    }
}

//int pieceAtLoc(int,int); //returns true/false if there is a piece at that location
void Board::turn(){
    status=status*-1;
}
//int checkbar(int); //checks to see if you have piece(s) on the bar
        //int checkmove(int,int); //checks to see if original location to new location is valid
        //void validateLoc(int,int); //(from, to) location, subtract/add num pieces and if necessary change color
void Board::checkBump(){
    opponent=0;
    place=0;

    /*for(int x=0; x<30; x++){
        if(list[x].getLocation()==startloc){
            startcolor=list[x].getColor();
        }
    }*/

    startcolor=loc[startloc].color;

    for(int x=0; x<30; x++){
            if(list[x].getLocation()==startloc+status*inputdie){
                if(list[x].getColor()!=startcolor){
                    opponent=1;
                }
            }
        }

        if(opponent!=1){
            place=1;
        }

        for(int i=0; i<30;i++){
                int x=list[i].getLocation();
                loc[x].numPieces++;
                loc[x].color=list[i].getColor();
            }

        if(opponent==1){
            if(loc[startloc+status*inputdie].numPieces==1){
                place=2;
            }
        }

        for(int i=0; i<30;i++){
                int x=list[i].getLocation();
                loc[x].numPieces=0;
                loc[x].color=list[i].getColor();
            }
}

void Board::bump(){
        for(int x=0; x<30; x++){
            if(list[x].getLocation()==startloc+status*inputdie){
                if(list[x].getColor()==1){
                    list[x].setPiece(26,1);
                    rbar++;
                }
                if(list[x].getColor()==2){
                    list[x].setPiece(27,2);
                    wbar++;
                }
            }
        }
}

int Board::diceRoll(){
    return (rand()%6)+1;
}

void Board::goFirst(){
    int redperson;
    int whiteperson;

    srand (time(NULL));
    redperson=6;//diceRoll();
    whiteperson=1;//diceRoll();
    cout<<endl<<endl<<"Player 1 rolled a "<<redperson<<" and Player 2 rolled a "<<whiteperson<<".";
    if(redperson>whiteperson){
        status=1;
    }
    else if(redperson<whiteperson){
        status=-1;
    }
    while(redperson==whiteperson){
        cout<<endl<<"Oops. You both rolled the same dice. Let's try again.";
        redperson=diceRoll();
        whiteperson=diceRoll();
        cout<<endl<<endl<<"Player 1 rolled a "<<redperson<<" and Player 2 rolled a "<<whiteperson<<".";
        if(redperson>whiteperson){
            status=1;
        }
        else if(redperson<whiteperson){
            status=-1;
        }
    }

}

void Board::move(){

    for(int loop=0; loop<1; loop++){

    int dice1=6;//diceRoll();
    int dice2=2;//diceRoll();
    int dice3=0;
    int dice4=0;
    int possible_moves;

            if(status==1){
                    for(int i=0; i<redpieces; i++){
                        if(list[i].getLocation()>18){
                           redhome++;
                        }
                        if(list[i].getLocation()==25){
                           redinhome++;
                        }

                    }
                    if(redhome==redpieces){
                        system("cls");
                        display();
                        cout<<endl<<endl<<"Player 1, you can move your pieces to home!"<<endl;
                    }
                    if(redinhome==redpieces){
                        system("cls");
                        display();
                        cout<<endl<<endl<<"Player 1, you won!!"<<endl;
                        endgame=1;
                        break;
                    }
            }
            if(status==-1){
                    for(int i=redpieces; i<redpieces+whitepieces; i++){
                        if(list[i].getLocation()<7){
                           whitehome++;
                        }
                        if(list[i].getLocation()==0){
                           whiteinhome++;
                        }
                    }
                    if(whitehome==whitepieces){
                        system("cls");
                        display();
                        cout<<endl<<endl<<"Player 2, you can move your pieces to home!"<<endl;
                    }
                    if(whiteinhome==whitepieces){
                        system("cls");
                        display();
                        cout<<endl<<endl<<"Player 2, you won!!"<<endl;
                        endgame=1;
                        break;
                    }
            }

    cout<<endl<<endl<<"It is player ";
    if(status==1)
        cout<<"1";
    if(status==-1)
        cout<<"2";
    cout<<"'s turn!"<<endl;

    if(dice1!=dice2)
        cout<<"You can move "<<dice1<<" or "<<dice2<<" or "<<dice1+dice2<<" spaces.";
    else{
        dice3=dice1;
        dice4=dice1;
        cout<<"You can move "<<dice1<<" or "<<dice1*2<<" or "<<dice1*3<<" or "<<dice1*4<<" spaces.";
    }
    cout<<endl;


        for(int closer=0; closer<30; closer++){
                if(status==1){
                    if(list[closer].getLocation()==26){
                        cout<<"You have a piece at bar, which is location 26. You will be forced to move that first."<<endl;
                        startloc=26;
                        movebarfirst=1;
                    }
                }
                if(status==-1){
                    if(list[closer].getLocation()==27){
                        cout<<"You have a piece at bar, which is location 27. You will be forced to move that first."<<endl;
                        startloc=27;
                        movebarfirst=1;
                    }
                }
            }


            cout<<"Choose a piece that you want to move. ";
            cin>>startloc;

            while((startloc!=list[0].getLocation())&&(startloc!=list[1].getLocation())&&(startloc!=list[2].getLocation())&&(startloc!=list[3].getLocation())&&(startloc!=list[4].getLocation())&&(startloc!=list[5].getLocation())&&(startloc!=list[6].getLocation())&&(startloc!=list[7].getLocation())&&(startloc!=list[8].getLocation())&&(startloc!=list[9].getLocation())&&(startloc!=list[10].getLocation())&&(startloc!=list[11].getLocation())&&(startloc!=list[12].getLocation())&&(startloc!=list[13].getLocation())&&(startloc!=list[14].getLocation())&&(startloc!=list[15].getLocation())&&(startloc!=list[16].getLocation())&&(startloc!=list[17].getLocation())&&(startloc!=list[18].getLocation())&&(startloc!=list[19].getLocation())&&(startloc!=list[20].getLocation())&&(startloc!=list[21].getLocation())&&(startloc!=list[22].getLocation())&&(startloc!=list[23].getLocation())&&(startloc!=list[24].getLocation())&&(startloc!=list[25].getLocation())&&(startloc!=list[26].getLocation())&&(startloc!=list[27].getLocation())&&(startloc!=list[28].getLocation())&&(startloc!=list[29].getLocation())){
                cout<<"Sorry, you can't move there. Try again: ";
                cin>>startloc;
            }

            if(status==1){

                for(int x=0; x<30; x++){
                    if(list[x].getLocation()==startloc){
                        position=x;
                    }
                }

                while(list[position].getColor()!=1){
                    cout<<"Sorry, that is your opponents piece. Try again: ";
                    cin>>startloc;
                    for(int x=0; x<30; x++){
                        if(list[x].getLocation()==startloc){
                            position=x;
                        }
                    }
                }

                while((startloc!=list[0].getLocation())&&(startloc!=list[1].getLocation())&&(startloc!=list[2].getLocation())&&(startloc!=list[3].getLocation())&&(startloc!=list[4].getLocation())&&(startloc!=list[5].getLocation())&&(startloc!=list[6].getLocation())&&(startloc!=list[7].getLocation())&&(startloc!=list[8].getLocation())&&(startloc!=list[9].getLocation())&&(startloc!=list[10].getLocation())&&(startloc!=list[11].getLocation())&&(startloc!=list[12].getLocation())&&(startloc!=list[13].getLocation())&&(startloc!=list[14].getLocation())&&(startloc!=list[15].getLocation())&&(startloc!=list[16].getLocation())&&(startloc!=list[17].getLocation())&&(startloc!=list[18].getLocation())&&(startloc!=list[19].getLocation())&&(startloc!=list[20].getLocation())&&(startloc!=list[21].getLocation())&&(startloc!=list[22].getLocation())&&(startloc!=list[23].getLocation())&&(startloc!=list[24].getLocation())&&(startloc!=list[25].getLocation())&&(startloc!=list[26].getLocation())&&(startloc!=list[27].getLocation())&&(startloc!=list[28].getLocation())&&(startloc!=list[29].getLocation())){
                    cout<<"Sorry, you can't move there. Try again: ";
                    cin>>startloc;
                }
            }

            if(status==-1){

                for(int x=0; x<30; x++){
                    if(list[x].getLocation()==startloc){
                        position=x;
                    }
                }

                while(list[position].getColor()!=2){
                    cout<<"Sorry, that is your opponents piece. Try again: ";
                    cin>>startloc;
                    for(int x=0; x<30; x++){
                        if(list[x].getLocation()==startloc){
                            position=x;
                        }
                    }
                }

                while((startloc!=list[0].getLocation())&&(startloc!=list[1].getLocation())&&(startloc!=list[2].getLocation())&&(startloc!=list[3].getLocation())&&(startloc!=list[4].getLocation())&&(startloc!=list[5].getLocation())&&(startloc!=list[6].getLocation())&&(startloc!=list[7].getLocation())&&(startloc!=list[8].getLocation())&&(startloc!=list[9].getLocation())&&(startloc!=list[10].getLocation())&&(startloc!=list[11].getLocation())&&(startloc!=list[12].getLocation())&&(startloc!=list[13].getLocation())&&(startloc!=list[14].getLocation())&&(startloc!=list[15].getLocation())&&(startloc!=list[16].getLocation())&&(startloc!=list[17].getLocation())&&(startloc!=list[18].getLocation())&&(startloc!=list[19].getLocation())&&(startloc!=list[20].getLocation())&&(startloc!=list[21].getLocation())&&(startloc!=list[22].getLocation())&&(startloc!=list[23].getLocation())&&(startloc!=list[24].getLocation())&&(startloc!=list[25].getLocation())&&(startloc!=list[26].getLocation())&&(startloc!=list[27].getLocation())&&(startloc!=list[28].getLocation())&&(startloc!=list[29].getLocation())){
                    cout<<"Sorry, you can't move there. Try again: ";
                    cin>>startloc;
                }
            }
                            if(movebarfirst==1){
                        cout<<"Sorry, you can only move location ";
                        if(status==1)
                            cout<<"26."<<endl;
                        if(status==-1)
                            cout<<"27."<<endl;
                        cout<<"What piece do you want to move? ";
                        cin>>startloc;
                    }


    if(dice3==0){

        cout<<"How many spaces do you want to move? ";
        cin>>inputdie;

        while((inputdie!=dice1)&&(inputdie!=dice2)&&(inputdie!=(dice1+dice2))){
            cout<<"Sorry, you can't move there. How many spaces do you want to move? ";
            cin>>inputdie;
        }


        checkBump();

            while(place==0){
                cout<<"Sorry, you can't move there. Choose a piece that you want to move. ";
                cin>>startloc;
                cout<<"How many spaces do you want to move? ";
                cin>>inputdie;
                checkBump();
            }

        if(place!=0){

            if(place==2){
                bump();
            }

                if(status==1){
                        if(startloc+status*inputdie>24){
                            if(redhome!=redpieces){
                                cout<<"You can't move there yet! Pick a new location: ";
                                cin>>startloc;
                                cout<<"How many spaces would you like to move? ";
                                cin>>inputdie;
                            }
                        }
                }
                if(status==-1){
                        if(startloc+status*inputdie<1){
                            if(whitehome!=whitepieces){
                                cout<<"You can't move there yet! Pick a new location: ";
                                cin>>startloc;
                                cout<<"How many spaces would you like to move? ";
                                cin>>inputdie;
                            }
                        }
                }

            if(inputdie!=dice1+dice2){
                for(int x=0; x<30; x++){
                  if(list[x].getLocation()==startloc){
                            if(startloc==26){
                                list[x].setPiece(inputdie,list[x].getColor());
                            }
                            if(startloc==0){
                                list[x].setPiece(25-inputdie,list[x].getColor());
                            }
                            else if(startloc+status*inputdie>25){
                                list[x].setPiece(25,list[x].getColor());
                            }
                            else if(startloc+status*inputdie<0){
                                list[x].setPiece(0,list[x].getColor());
                            }
                            else{
                                list[x].setPiece(startloc+status*inputdie,list[x].getColor());
                            }
                            break;

                    }
                }


                system("cls");
                display();

                redhome=0;
                whitehome=0;
                redinhome=0;
                whiteinhome=0;

                if(status==1){
                        for(int i=0; i<redpieces; i++){
                            if(list[i].getLocation()>18){
                               redhome++;
                            }
                            if(list[i].getLocation()==25){
                               redinhome++;
                            }
                        }
                        if(redhome==redpieces){
                            system("cls");
                            display();
                            cout<<endl<<endl<<"Player 1, you can move your pieces to home!"<<endl;
                        }
                        if(redinhome==redpieces){
                            system("cls");
                            display();
                            cout<<endl<<endl<<"Player 1, you won!!"<<endl;
                            endgame=1;
                            break;

                        }
                }
                if(status==-1){
                        for(int i=redpieces; i<redpieces+whitepieces; i++){
                            if(list[i].getLocation()<7){
                               whitehome++;
                            }
                            if(list[i].getLocation()==0){
                               whiteinhome++;
                            }
                        }
                        if(whitehome==whitepieces){
                            system("cls");
                            display();
                            cout<<endl<<endl<<"Player 2, you can move your pieces to home!"<<endl;
                        }
                        if(whiteinhome==whitepieces){
                            system("cls");
                            display();
                            cout<<endl<<endl<<"Player 2, you won!!"<<endl;
                            endgame=1;
                            break;
                        }
                }

                cout<<endl<<"You will move "<<dice1+dice2-inputdie<<" spaces."<<endl;

        for(int closer=0; closer<30; closer++){
                if(status==1){
                    if(list[closer].getLocation()==26){
                        cout<<"You have a piece at bar, which is location 26. You will be forced to move that first."<<endl;
                        startloc=26;
                        movebarfirst=1;
                    }
                }
                if(status==-1){
                    if(list[closer].getLocation()==27){
                        cout<<"You have a piece at bar, which is location 27. You will be forced to move that first."<<endl;
                        startloc=27;
                        movebarfirst=1;
                    }
                }
            }


                cout<<"Choose a piece that you want to move. ";
                cin>>startloc;

                while((startloc!=list[0].getLocation())&&(startloc!=list[1].getLocation())&&(startloc!=list[2].getLocation())&&(startloc!=list[3].getLocation())&&(startloc!=list[4].getLocation())&&(startloc!=list[5].getLocation())&&(startloc!=list[6].getLocation())&&(startloc!=list[7].getLocation())&&(startloc!=list[8].getLocation())&&(startloc!=list[9].getLocation())&&(startloc!=list[10].getLocation())&&(startloc!=list[11].getLocation())&&(startloc!=list[12].getLocation())&&(startloc!=list[13].getLocation())&&(startloc!=list[14].getLocation())&&(startloc!=list[15].getLocation())&&(startloc!=list[16].getLocation())&&(startloc!=list[17].getLocation())&&(startloc!=list[18].getLocation())&&(startloc!=list[19].getLocation())&&(startloc!=list[20].getLocation())&&(startloc!=list[21].getLocation())&&(startloc!=list[22].getLocation())&&(startloc!=list[23].getLocation())&&(startloc!=list[24].getLocation())&&(startloc!=list[25].getLocation())&&(startloc!=list[26].getLocation())&&(startloc!=list[27].getLocation())&&(startloc!=list[28].getLocation())&&(startloc!=list[29].getLocation())){
                cout<<"Sorry, you can't move there. Try again: ";
                cin>>startloc;
            }
            if(status==1){

                for(int x=0; x<30; x++){
                    if(list[x].getLocation()==startloc){
                        position=x;
                    }
                }

                while(list[position].getColor()!=1){
                    cout<<"Sorry, that is your opponents piece. Try again: ";
                    cin>>startloc;
                    for(int x=0; x<30; x++){
                        if(list[x].getLocation()==startloc){
                            position=x;
                        }
                    }
                }

                while((startloc!=list[0].getLocation())&&(startloc!=list[1].getLocation())&&(startloc!=list[2].getLocation())&&(startloc!=list[3].getLocation())&&(startloc!=list[4].getLocation())&&(startloc!=list[5].getLocation())&&(startloc!=list[6].getLocation())&&(startloc!=list[7].getLocation())&&(startloc!=list[8].getLocation())&&(startloc!=list[9].getLocation())&&(startloc!=list[10].getLocation())&&(startloc!=list[11].getLocation())&&(startloc!=list[12].getLocation())&&(startloc!=list[13].getLocation())&&(startloc!=list[14].getLocation())&&(startloc!=list[15].getLocation())&&(startloc!=list[16].getLocation())&&(startloc!=list[17].getLocation())&&(startloc!=list[18].getLocation())&&(startloc!=list[19].getLocation())&&(startloc!=list[20].getLocation())&&(startloc!=list[21].getLocation())&&(startloc!=list[22].getLocation())&&(startloc!=list[23].getLocation())&&(startloc!=list[24].getLocation())&&(startloc!=list[25].getLocation())&&(startloc!=list[26].getLocation())&&(startloc!=list[27].getLocation())&&(startloc!=list[28].getLocation())&&(startloc!=list[29].getLocation())){
                    cout<<"Sorry, you can't move there. Try again: ";
                    cin>>startloc;
                }
            }

            if(status==-1){

                for(int x=0; x<30; x++){
                    if(list[x].getLocation()==startloc){
                        position=x;
                    }
                }

                while(list[position].getColor()!=2){
                    cout<<"Sorry, that is your opponents piece. Try again: ";
                    cin>>startloc;
                    for(int x=0; x<30; x++){
                        if(list[x].getLocation()==startloc){
                            position=x;
                        }
                    }
                }

                while((startloc!=list[0].getLocation())&&(startloc!=list[1].getLocation())&&(startloc!=list[2].getLocation())&&(startloc!=list[3].getLocation())&&(startloc!=list[4].getLocation())&&(startloc!=list[5].getLocation())&&(startloc!=list[6].getLocation())&&(startloc!=list[7].getLocation())&&(startloc!=list[8].getLocation())&&(startloc!=list[9].getLocation())&&(startloc!=list[10].getLocation())&&(startloc!=list[11].getLocation())&&(startloc!=list[12].getLocation())&&(startloc!=list[13].getLocation())&&(startloc!=list[14].getLocation())&&(startloc!=list[15].getLocation())&&(startloc!=list[16].getLocation())&&(startloc!=list[17].getLocation())&&(startloc!=list[18].getLocation())&&(startloc!=list[19].getLocation())&&(startloc!=list[20].getLocation())&&(startloc!=list[21].getLocation())&&(startloc!=list[22].getLocation())&&(startloc!=list[23].getLocation())&&(startloc!=list[24].getLocation())&&(startloc!=list[25].getLocation())&&(startloc!=list[26].getLocation())&&(startloc!=list[27].getLocation())&&(startloc!=list[28].getLocation())&&(startloc!=list[29].getLocation())){
                    cout<<"Sorry, you can't move there. Try again: ";
                    cin>>startloc;
                }
            }
                    if(movebarfirst==1){
                        cout<<"Sorry, you can only move location ";
                        if(status==1)
                            cout<<"26."<<endl;
                        if(status==-1)
                            cout<<"27."<<endl;
                        cout<<"What piece do you want to move? ";
                        cin>>startloc;
                    }
                inputdie=dice1+dice2-inputdie;

                checkBump();

                while(place==0){
                    cout<<"Sorry, you can't move there. Choose a piece you want to move. ";
                    cin>>startloc;
                    checkBump();

                }

                if(place==2){
                    bump();
                }

                if(status==1){
                        if(startloc+status*inputdie>24){
                            if(redhome!=redpieces){
                                cout<<"You can't move there yet! Pick a new location: ";
                                cin>>startloc;
                                cout<<"How many spaces would you like to move? ";
                                cin>>inputdie;
                            }
                        }
                }
                if(status==-1){
                        if(startloc+status*inputdie<1){
                            if(whitehome!=whitepieces){
                                cout<<"You can't move there yet! Pick a new location: ";
                                cin>>startloc;
                                cout<<"How many spaces would you like to move? ";
                                cin>>inputdie;
                            }
                        }
                }

            for(int x=0; x<30; x++){
                  if(list[x].getLocation()==startloc){
                            if(startloc==26){
                                list[x].setPiece(inputdie,list[x].getColor());
                            }
                            if(startloc==0){
                                list[x].setPiece(25-inputdie,list[x].getColor());
                            }
                            else if(startloc+status*inputdie>25){
                                list[x].setPiece(25,list[x].getColor());
                            }
                            else if(startloc+status*inputdie<0){
                                list[x].setPiece(0,list[x].getColor());
                            }
                            else{
                                list[x].setPiece(startloc+status*inputdie,list[x].getColor());
                            }
                            break;

                    }
                }
                system("cls");
                display();

                redhome=0;
                whitehome=0;
                redinhome=0;
                whiteinhome=0;

                if(status==1){
                        for(int i=0; i<redpieces; i++){
                            if(list[i].getLocation()>18){
                               redhome++;
                            }
                            if(list[i].getLocation()==25){
                               redinhome++;
                            }
                        }
                        if(redhome==redpieces){
                            system("cls");
                            display();
                            cout<<endl<<endl<<"Player 1, you can move your pieces to home!"<<endl;
                        }
                        if(redinhome==redpieces){
                            system("cls");
                            display();
                            cout<<endl<<endl<<"Player 1, you won!!"<<endl;
                            endgame=1;
                            break;
                        }
                }
                if(status==-1){
                        for(int i=redpieces; i<redpieces+whitepieces; i++){
                            if(list[i].getLocation()<7){
                               whitehome++;
                            }
                            if(list[i].getLocation()==0){
                               whiteinhome++;
                            }
                        }
                        if(whitehome==whitepieces){
                            system("cls");
                            display();
                            cout<<endl<<endl<<"Player 2, you can move your pieces to home!"<<endl;
                        }
                        if(whiteinhome==whitepieces){
                            system("cls");
                            display();
                            cout<<endl<<endl<<"Player 2, you won!!"<<endl;
                            endgame=1;
                            break;
                        }
                }


            }
            else{
                for(int x=0; x<30; x++){
                  if(list[x].getLocation()==startloc){
                            if(startloc==26){
                                list[x].setPiece(inputdie,list[x].getColor());
                            }
                            if(startloc==0){
                                list[x].setPiece(25-inputdie,list[x].getColor());
                            }
                            else if(startloc+status*inputdie>25){
                                list[x].setPiece(25,list[x].getColor());
                            }
                            else if(startloc+status*inputdie<0){
                                list[x].setPiece(0,list[x].getColor());
                            }
                            else{
                                list[x].setPiece(startloc+status*inputdie,list[x].getColor());
                            }
                            break;

                    }
                }
                system("cls");
                display();

                redhome=0;
                whitehome=0;
                redinhome=0;
                whiteinhome=0;

                if(status==1){
                    for(int i=0; i<redpieces; i++){
                        if(list[i].getLocation()>18){
                           redhome++;
                        }
                        if(list[i].getLocation()==25){
                           redinhome++;
                        }

                    }
                    if(redhome==redpieces){
                        system("cls");
                        display();
                        cout<<endl<<endl<<"Player 1, you can move your pieces to home!"<<endl;
                    }
                    if(redinhome==redpieces){
                        system("cls");
                        display();
                        cout<<endl<<endl<<"Player 1, you won!!"<<endl;
                        endgame=1;
                        break;
                    }
            }
            if(status==-1){
                    for(int i=redpieces; i<redpieces+whitepieces; i++){
                        if(list[i].getLocation()<7){
                           whitehome++;
                        }
                        if(list[i].getLocation()==0){
                           whiteinhome++;
                        }
                    }
                    if(whitehome==whitepieces){
                        system("cls");
                        display();
                        cout<<endl<<endl<<"Player 2, you can move your pieces to home!"<<endl;
                    }
                    if(whiteinhome==whitepieces){
                        system("cls");
                        display();
                        cout<<endl<<endl<<"Player 2, you won!!"<<endl;
                        endgame=1;
                        break;
                    }
            }

            }
        }
    }
    else{
        cout<<"How many spaces do you want to move? ";
        cin>>inputdie;

        while((inputdie!=dice1)&&(inputdie!=dice1*2)&&(inputdie!=dice1*3)&&(inputdie!=dice1*4)){
            cout<<"Sorry, you can't move there. How many spaces do you want to move? ";
            cin>>inputdie;
        }

        checkBump();

            while(place==0){
                cout<<"Sorry, you can't move there. Choose a piece that you want to move. ";
                cin>>startloc;
                cout<<"How many spaces do you want to move? ";
                cin>>inputdie;
                checkBump();
            }

        if(place!=0){

            if(place==2){
                bump();
            }

                if(status==1){
                        if(startloc+status*inputdie>24){
                            if(redhome!=redpieces){
                                cout<<"You can't move there yet! Pick a new location: ";
                                cin>>startloc;
                                cout<<"How many spaces would you like to move? ";
                                cin>>inputdie;
                            }
                        }
                }
                if(status==-1){
                        if(startloc+status*inputdie<1){
                            if(whitehome!=whitepieces){
                                cout<<"You can't move there yet! Pick a new location: ";
                                cin>>startloc;
                                cout<<"How many spaces would you like to move? ";
                                cin>>inputdie;
                            }
                        }
                }
        }

                for(int x=0; x<30; x++){
                  if(list[x].getLocation()==startloc){
                            if(startloc==26){
                                list[x].setPiece(inputdie,list[x].getColor());
                            }
                            if(startloc==0){
                                list[x].setPiece(25-inputdie,list[x].getColor());
                            }
                            else if(startloc+status*inputdie>25){
                                list[x].setPiece(25,list[x].getColor());
                            }
                            else if(startloc+status*inputdie<0){
                                list[x].setPiece(0,list[x].getColor());
                            }
                            else{
                                list[x].setPiece(startloc+status*inputdie,list[x].getColor());
                            }
                            break;

                    }
                }

            system("cls");
            display();

            possible_moves=dice1*4;
            possible_moves=possible_moves-inputdie;

        while(possible_moves!=0){
                redhome=0;
                whitehome=0;
                redinhome=0;
                whiteinhome=0;

            if(status==1){
                        for(int i=0; i<redpieces; i++){
                            if(list[i].getLocation()>18){
                               redhome++;
                            }
                            if(list[i].getLocation()==25){
                               redinhome++;
                            }
                        }
                        if(redhome==redpieces){
                            system("cls");
                            display();
                            cout<<endl<<endl<<"Player 1, you can move your pieces to home!"<<endl;
                        }
                        if(redinhome==redpieces){
                            system("cls");
                            display();
                            cout<<endl<<endl<<"Player 1, you won!!"<<endl;
                            endgame=1;
                            break;
                        }
                }
                if(status==-1){
                        for(int i=redpieces; i<redpieces+whitepieces; i++){
                            if(list[i].getLocation()<7){
                               whitehome++;
                            }
                            if(list[i].getLocation()==0){
                               whiteinhome++;
                            }
                        }
                        if(whitehome==whitepieces){
                            system("cls");
                            display();
                            cout<<endl<<endl<<"Player 2, you can move your pieces to home!"<<endl;
                        }
                        if(whiteinhome==whitepieces){
                            system("cls");
                            display();
                            cout<<endl<<endl<<"Player 2, you won!!"<<endl;
                            endgame=1;
                            break;
                        }
                }


            cout<<endl<<"You can move "<<dice1;
            if(possible_moves==dice1*2)
                cout<<" or "<<dice1*2;
            if(possible_moves==dice1*3)
                cout<<" or "<<dice1*2<<" or "<<dice1*3;
            cout<<" spaces. ";
            cout<<"Choose a piece that you want to move. ";
            cin>>startloc;

            while((startloc!=list[0].getLocation())&&(startloc!=list[1].getLocation())&&(startloc!=list[2].getLocation())&&(startloc!=list[3].getLocation())&&(startloc!=list[4].getLocation())&&(startloc!=list[5].getLocation())&&(startloc!=list[6].getLocation())&&(startloc!=list[7].getLocation())&&(startloc!=list[8].getLocation())&&(startloc!=list[9].getLocation())&&(startloc!=list[10].getLocation())&&(startloc!=list[11].getLocation())&&(startloc!=list[12].getLocation())&&(startloc!=list[13].getLocation())&&(startloc!=list[14].getLocation())&&(startloc!=list[15].getLocation())&&(startloc!=list[16].getLocation())&&(startloc!=list[17].getLocation())&&(startloc!=list[18].getLocation())&&(startloc!=list[19].getLocation())&&(startloc!=list[20].getLocation())&&(startloc!=list[21].getLocation())&&(startloc!=list[22].getLocation())&&(startloc!=list[23].getLocation())&&(startloc!=list[24].getLocation())&&(startloc!=list[25].getLocation())&&(startloc!=list[26].getLocation())&&(startloc!=list[27].getLocation())&&(startloc!=list[28].getLocation())&&(startloc!=list[29].getLocation())){
                cout<<"Sorry, you can't move there. Try again: ";
                cin>>startloc;
            }
        if(status==1){

            for(int x=0; x<30; x++){
                if(list[x].getLocation()==startloc){
                    position=x;
                }
            }

            while(list[position].getColor()!=1){
                cout<<"Sorry, that is your opponents piece. Try again: ";
                cin>>startloc;
                for(int x=0; x<30; x++){
                    if(list[x].getLocation()==startloc){
                        position=x;
                    }
                }
            }

            while((startloc!=list[0].getLocation())&&(startloc!=list[1].getLocation())&&(startloc!=list[2].getLocation())&&(startloc!=list[3].getLocation())&&(startloc!=list[4].getLocation())&&(startloc!=list[5].getLocation())&&(startloc!=list[6].getLocation())&&(startloc!=list[7].getLocation())&&(startloc!=list[8].getLocation())&&(startloc!=list[9].getLocation())&&(startloc!=list[10].getLocation())&&(startloc!=list[11].getLocation())&&(startloc!=list[12].getLocation())&&(startloc!=list[13].getLocation())&&(startloc!=list[14].getLocation())&&(startloc!=list[15].getLocation())&&(startloc!=list[16].getLocation())&&(startloc!=list[17].getLocation())&&(startloc!=list[18].getLocation())&&(startloc!=list[19].getLocation())&&(startloc!=list[20].getLocation())&&(startloc!=list[21].getLocation())&&(startloc!=list[22].getLocation())&&(startloc!=list[23].getLocation())&&(startloc!=list[24].getLocation())&&(startloc!=list[25].getLocation())&&(startloc!=list[26].getLocation())&&(startloc!=list[27].getLocation())&&(startloc!=list[28].getLocation())&&(startloc!=list[29].getLocation())){
                cout<<"Sorry, you can't move there. Try again: ";
                cin>>startloc;
            }
        }

        if(status==-1){

            for(int x=0; x<30; x++){
                if(list[x].getLocation()==startloc){
                    position=x;
                }
            }

            while(list[position].getColor()!=2){
                cout<<"Sorry, that is your opponents piece. Try again: ";
                cin>>startloc;
                for(int x=0; x<30; x++){
                    if(list[x].getLocation()==startloc){
                        position=x;
                    }
                }
            }

            while((startloc!=list[0].getLocation())&&(startloc!=list[1].getLocation())&&(startloc!=list[2].getLocation())&&(startloc!=list[3].getLocation())&&(startloc!=list[4].getLocation())&&(startloc!=list[5].getLocation())&&(startloc!=list[6].getLocation())&&(startloc!=list[7].getLocation())&&(startloc!=list[8].getLocation())&&(startloc!=list[9].getLocation())&&(startloc!=list[10].getLocation())&&(startloc!=list[11].getLocation())&&(startloc!=list[12].getLocation())&&(startloc!=list[13].getLocation())&&(startloc!=list[14].getLocation())&&(startloc!=list[15].getLocation())&&(startloc!=list[16].getLocation())&&(startloc!=list[17].getLocation())&&(startloc!=list[18].getLocation())&&(startloc!=list[19].getLocation())&&(startloc!=list[20].getLocation())&&(startloc!=list[21].getLocation())&&(startloc!=list[22].getLocation())&&(startloc!=list[23].getLocation())&&(startloc!=list[24].getLocation())&&(startloc!=list[25].getLocation())&&(startloc!=list[26].getLocation())&&(startloc!=list[27].getLocation())&&(startloc!=list[28].getLocation())&&(startloc!=list[29].getLocation())){
                cout<<"Sorry, you can't move there. Try again: ";
                cin>>startloc;
            }
        }

        cout<<"How many spaces do you want to move? ";
        cin>>inputdie;

        if(possible_moves==dice1*3){
            while((inputdie!=dice1)&&(inputdie!=dice1*2)&&(inputdie!=dice1*3)){
                cout<<"Sorry, you can't move there. How many spaces do you want to move? ";
                cin>>inputdie;
            }
        }
        if(possible_moves==dice1*2){
            while((inputdie!=dice1)&&(inputdie!=dice1*2)){
                cout<<"Sorry, you can't move there. How many spaces do you want to move? ";
                cin>>inputdie;
            }
        }
        if(possible_moves==dice1){
            inputdie=dice1;
        }

        checkBump();

            while(place==0){
                cout<<"Sorry, you can't move there. Choose a piece that you want to move. ";
                cin>>startloc;
                cout<<"How many spaces do you want to move? ";
                cin>>inputdie;
                checkBump();
            }

        if(place!=0){

            if(place==2){
                bump();
            }

                if(status==1){
                        if(startloc+status*inputdie>24){
                            if(redhome!=redpieces){
                                cout<<"You can't move there yet! Pick a new location: ";
                                cin>>startloc;
                                cout<<"How many spaces would you like to move? ";
                                cin>>inputdie;
                            }
                        }
                }
                if(status==-1){
                        if(startloc+status*inputdie<1){
                            if(whitehome!=whitepieces){
                                cout<<"You can't move there yet! Pick a new location: ";
                                cin>>startloc;
                                cout<<"How many spaces would you like to move? ";
                                cin>>inputdie;
                            }
                        }
                }
                for(int x=0; x<30; x++){
                  if(list[x].getLocation()==startloc){
                            if(startloc==26){
                                list[x].setPiece(inputdie,list[x].getColor());
                            }
                            if(startloc==0){
                                list[x].setPiece(25-inputdie,list[x].getColor());
                            }
                            else if(startloc+status*inputdie>25){
                                list[x].setPiece(25,list[x].getColor());
                            }
                            else if(startloc+status*inputdie<0){
                                list[x].setPiece(0,list[x].getColor());
                            }
                            else{
                                list[x].setPiece(startloc+status*inputdie,list[x].getColor());
                            }
                            break;

                    }
                }

                system("cls");
                display();

                if(status==1){
                        for(int i=0; i<redpieces; i++){
                            if(list[i].getLocation()>18){
                               redhome++;
                            }
                            if(list[i].getLocation()==25){
                               redinhome++;
                            }
                        }
                        if(redhome==redpieces){
                            system("cls");
                            display();
                            cout<<endl<<endl<<"Player 1, you can move your pieces to home!"<<endl;
                        }
                        if(redinhome==redpieces){
                            system("cls");
                            display();
                            cout<<endl<<endl<<"Player 1, you won!!"<<endl;
                            endgame=1;
                            break;
                        }
                }
                if(status==-1){
                        for(int i=redpieces; i<redpieces+whitepieces; i++){
                            if(list[i].getLocation()<7){
                               whitehome++;
                            }
                            if(list[i].getLocation()==0){
                               whiteinhome++;
                            }
                        }
                        if(whitehome==whitepieces){
                            system("cls");
                            display();
                            cout<<endl<<endl<<"Player 2, you can move your pieces to home!"<<endl;
                        }
                        if(whiteinhome==whitepieces){
                            system("cls");
                            display();
                            cout<<endl<<endl<<"Player 2, you won!!"<<endl;
                            endgame=1;
                            break;
                        }
                }


        }
        possible_moves=possible_moves-inputdie;
    }
}
}
}

void Board::display(){



            for(int i=0; i<30;i++){
                int x=list[i].getLocation();
                loc[x].numPieces++;
                loc[x].color=list[i].getColor();
            }

            for(int i=0; i<12; i++){
                gotoxy(5*i,0);cout<<i+1;
                if(loc[i+1].numPieces>0){
                    if(loc[i+1].color==1){
                        gotoxy(5*i,1);cout<<"R";
                    }
                     if(loc[i+1].color==2){
                        gotoxy(5*i,1);cout<<"W";
                    }
                    gotoxy(5*i,2);cout<<loc[i+1].numPieces;
                }
            }

            gotoxy(0,5);cout<<"Number of pieces on Bar:    Red-     White- ";
            gotoxy(32,5);cout<<rbar;
            gotoxy(43,5);cout<<wbar;

            int j=0;

            for(int i=24; i>12; i--){
                if(loc[i].numPieces>0){
                    gotoxy(5*j,7);cout<<loc[i].numPieces;
                    if(loc[i].color==1){
                        gotoxy(5*j,8);cout<<"R";
                    }
                     if(loc[i].color==2){
                        gotoxy(5*j,8);cout<<"W";
                    }
                }

                gotoxy(5*j,9);cout<<i;
                j++;
            }

    for(int i=0; i<30;i++){
                int x=list[i].getLocation();
                loc[x].numPieces=0;
                loc[x].color=list[i].getColor();
            }

}

void Board::execute(){
    cout<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
    display();
    goFirst();
    for(int loop=0; loop<2; loop++){
        move();
        cout<<endl;
        if(endgame==1)
            break;
        turn();
    }
}

int main(){

    Board b;
    b.execute();
    return 0;
}
