//
//  CodeGate.cpp
//  NoCompromiseMABE
//
//  Created by Arend Hintze on 2020-08-28.
//  Copyright © 2020 ArendHintze. All rights reserved.
//

#include "CodeGate.hpp"



CodeGate::CodeGate(std::vector<int> _ins,std::vector<int> _outs,std::vector<std::vector<int>> _logic,std::vector<int> _theCode){
    ins=_ins;
    outs=_outs;
    table=_logic;
    for(int i:_theCode)
        instructions.push_back(Instruction(i));
    for(int i=0;i<(int)table.size();i++){
        double sum=0.0;
        for(int o=0;o<(int)table[i].size();o++){
            table[i][o]=abs(table[i][o]);
            sum+=table[i][o];
        }
        if(sum>0.0)
            for(int o=0;o<(int)table[i].size();o++)
                table[i][o]/=sum;
        else
            for(int o=0;o<(int)table[i].size();o++)
                table[i][o]=1.0/(double)table[i].size();

    }
    iStack.assign(1,255);
    oStack.assign(1,255);
 }


void CodeGate::update(std::vector<double> &from,std::vector<double> &to){
    int I=0;
    int O=0;
    int totalStepsAllowed=256;
    reg.assign(16,0);
    for(int i=0;i<(int)ins.size();i++){
        I=(I<<1)+bit(from[ins[i]]);
        reg[i]=from[ins[i]];
    }
    stack.assign(1,(double)I);
    int steps=0;
    int ip=0;
    while(steps<totalStepsAllowed){
        Instruction instruction=instructions[ip];
        /*
        std::cout<<instruction.i<<" "<<instruction.f<<" "
        <<(int)instruction.c[0]<<" "
        <<(int)instruction.c[1]<<" "
        <<(int)instruction.c[2]<<" "
        <<(int)instruction.c[3]<<" "
//        <<(int)instruction.c[4]<<" "
//        <<(int)instruction.c[5]<<" "
//        <<(int)instruction.c[6]<<" "
//        <<(int)instruction.c[7]<<" "
        <<std::endl;*/
        //std::cout<<(int)instruction.addrField.command<<" "<<(int)instruction.addrField.addr0<<" "<<(int)instruction.addrField.addr1<<" "<<(int)instruction.addrField.addr2<<std::endl;
        //std::cout<<(int)instruction.wideAddrField._aa<<" "<<(int)instruction.wideAddrField._bb<<std::endl;
        switch(instruction.addrField.command){
            case 0: /* EXIT */
                steps+=totalStepsAllowed;
                break;
            case 1: //MOV L<-R
                reg[instruction.addrField.addr0]=reg[instruction.addrField.addr1];
                break;
            case 2: //MEM
                if(instruction.addrField.f0==1)
                    reg[instruction.addrField.addr0]=table[instruction.addrField.addr1%(int)ins.size()][instruction.addrField.addr2%(int)outs.size()];
                else
                    table[instruction.addrField.addr1%(int)ins.size()][instruction.addrField.addr2%(int)outs.size()]=reg[instruction.addrField.addr0];
                break;
            case 3: //Temp
                if(instruction.addrField.f0==1)
                    reg[instruction.addrField.addr0]=iStack[instruction.wideAddrField._aa%(int)iStack.size()];
                else
                    reg[instruction.addrField.addr0]=oStack[instruction.wideAddrField._aa%(int)oStack.size()];
                break;
            case 4: //Jmp
                ip+=instruction.wideAddrField._bb;
                break;
            case 5: //RAND
                reg[instruction.addrField.addr0]=(unsigned char)Random::getInt(0, 255);
                break;
            case 6: //COMP
                if(reg[instruction.addrField.addr0]==reg[instruction.addrField.addr1])
                    ip++;
                break;
            case 7: //
                break;
            case 8: // PUSH
                stack.push_back(reg[instruction.addrField.addr0]);
                break;
            case 9: //POP
                if((int)stack.size()>0){
                    reg[instruction.addrField.addr0]=stack.back();
                    stack.pop_back();
                }
                break;
            case 10: //ADD
                reg[instruction.addrField.addr0]+=reg[instruction.addrField.addr1];
                break;
            case 11: //SUB
                reg[instruction.addrField.addr0]-=reg[instruction.addrField.addr1];
                break;
            case 12: //MULT
                reg[instruction.addrField.addr0]+=reg[instruction.addrField.addr1];
                break;
            case 13: //CLR
                reg[instruction.addrField.addr0]=0;
                break;
            case 14: //CONST
                reg[instruction.addrField.addr0]=instruction.wideAddrField._bb;
                break;
            case 15: //LOGIC
            {
                unsigned char b=0;
                for(int l=0;l<8;l++){
                    int a=(reg[instruction.addrField.addr0]>>l)&1;
                    a+=((reg[instruction.addrField.addr1]>>l)&1)*2;
                    b=(b<<1)+((reg[instruction.addrField.addr2]>>a)&1);
                }
                reg[instruction.addrField.addr0]=b;
            }
                break;
}
         ip=(ip+1)&63;
        steps++;
    }
    for(int o=0;o<(int)outs.size();o++){
        to[outs[o]]+=reg[4+o];
        O+=bit(reg[4+o])<<o;
    }
    iStack.push_back(I);
    oStack.push_back(O);
}

void CodeGate::reset(){
}

std::shared_ptr<Gate> CodeGate::clone() {
    std::vector<int> theCode;
    for(auto i:instructions)
        theCode.push_back(i.i);
    std::shared_ptr<CodeGate> G=std::make_shared<CodeGate>(ins,outs,table,theCode);
    std::cout<<"code gate clone() not implemented"<<std::endl;
    return std::dynamic_pointer_cast<Gate>(G);
}

void CodeGate::show(){
}


