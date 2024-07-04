#include "cpu/minor/cuscon.hh"
#include "cpu/minor/dyn_inst.hh"
#include "debug/MinorExecute.hh"
//info索引
#define IST 0
#define IDX1 (IST+1)
#define IDX2 (IST+2)
#define IDX3 (IST+3)
#define TOTALIDX (IDX3+1)
//用指令编号去索引busyVec
//指令编号为0，用VEC编号去索引ldBusyVec
#define VLOAD 0
#define AAMUL02 1
#define AAMUL12 2
#define AAMUL21 3
#define AAMUL31 4
#define AAMUL1221 5
#define TRIADD012 6
#define TRIADD321 7
#define OACC 8
#define VSTORE 9
#define TOTALIST 10
//vec:新增tmp
#define OUTVEC  8
#define TMPA 9
#define TMPB 10
#define TOTALVEC 11
//BUSY指示正在被写
#define BUSY 0
#define DONE 1

namespace gem5 
{

namespace minor 
{

CustomControl::CustomControl() 
    : controlVec(TOTALVEC, 0), busyVec(TOTALIST, false) ,ldBusyVec(TOTALVEC, false)
{   
}

CustomControl::~CustomControl() 
{
    //这里不需要做任何事情，因为向量是自动管理内存的
}

void 
CustomControl::setCtrlVec(int* ctrlV)
{
    for (int i=0;i<TOTALVEC;i++)
    {
        setVal(i,ctrlV[i]);
    }
}

int* 
CustomControl::getCtrlVec()
{
    int* re = new int[TOTALVEC];
    for (int i=0;i<TOTALVEC;i++)
    {
        re[i]=controlVec[i];
    }
    return re;
}

/*
 * 根据指令的类型，获取指令信息
 *      issueval【指令发射vec阈值】
 *      doneval 【指令完成vec值】
 *      instnum 【指令类型编号】
 *      idx     【指令所需vec索引】   
 * OACC稍微有点说法，因为两组vec的issueval和doneval不太一样，具体在ckinfo和doneist中去处理
 */
int* 
CustomControl::getInfo(MinorDynInstPtr inst)
{
    //注意这里是按照vload去初始化的
    int* returnIdx = new int[TOTALIDX];
    returnIdx[IST] = VLOAD;
    returnIdx[IDX1] = TOTALVEC;
    returnIdx[IDX2] = TOTALVEC;
    returnIdx[IDX3] = TOTALVEC;
    //VLOAD
    if(inst->isVLoad()){
        RegIndex destIdx = inst->cDestIdx();
        returnIdx[IDX1]=destIdx;
        DPRINTF(MinorExecute, "VLOAD: dest vec is %i\n", returnIdx[IDX1]);
    }
    //AAMul
    else if (inst->isAAMul())
    {
        if (inst->isAAMul02())
        {
            returnIdx[IST]=AAMUL02;
            returnIdx[IDX1]=0;
            returnIdx[IDX2]=2;
            DPRINTF(MinorExecute, "AAMUL02\n");
        }
        else if (inst->isAAMul31())
        {
            returnIdx[IST]=AAMUL31;
            returnIdx[IDX1]=3;
            returnIdx[IDX2]=1;
            DPRINTF(MinorExecute, "AAMUL31\n");
        }    
        else if (inst->isAAMul12())
        {
            returnIdx[IST]=AAMUL12;
            returnIdx[IDX1]=1;
            returnIdx[IDX2]=2;
            DPRINTF(MinorExecute, "AAMUL12\n");
        }
        else if (inst->isAAMul21())
        {
            returnIdx[IST]=AAMUL21;
            returnIdx[IDX1]=2;
            returnIdx[IDX2]=1;
            DPRINTF(MinorExecute, "AAMUL21\n");
        }
    }
    //TriAdd OACC
    else if (inst->isTriAdd())
    {
        if (inst->isTriAdd012())
        {
            returnIdx[IST]=TRIADD012;
            returnIdx[IDX1]=0;
            returnIdx[IDX2]=TMPA;
            returnIdx[IDX3]=TMPB;
            DPRINTF(MinorExecute, "TRIADD012\n");
        }
        else if (inst->isTriAdd321())
        {
            returnIdx[IST]=TRIADD321;
            returnIdx[IDX1]=3;
            returnIdx[IDX2]=TMPB;
            returnIdx[IDX3]=TMPA;
            DPRINTF(MinorExecute, "TRIADD321\n");
        }
    }
    else if (inst->isOacc())
    {
        returnIdx[IST]=OACC;
        returnIdx[IDX1]=OUTVEC;
        returnIdx[IDX2]=0;
        returnIdx[IDX3]=3;
        DPRINTF(MinorExecute, "OACC\n");
    }
    //VSTORE
    else if (inst->isVStore())
    {
        returnIdx[IST]=VSTORE;
        returnIdx[IDX1]=OUTVEC;
        DPRINTF(MinorExecute, "VSTORE\n");
    }
    DPRINTF(MinorExecute, "get custom inst infomation: idx1 %i,idx2 %i,idx3 %i\n", returnIdx[IDX1],returnIdx[IDX2],returnIdx[IDX3]);
    return returnIdx;
}
int
CustomControl::numOfIdx(int* info)
{
    if (info[IDX2]==TOTALVEC) return 1;
    else if(info[IDX3]==TOTALVEC) return 2;
    else return 3;
}

//issue前：检查vec进度
bool 
CustomControl::ckVal(RegIndex idx,int val)
{
    //DPRINTF(MinorExecute, "custom check ctrlVec: vec %i ,issue val %i,actual val %i\n", idx,val,controlVec[idx]);
    return (controlVec[idx]==val);
}
//done后：设置vec进度
void
CustomControl::setVal(RegIndex idx,int val)
{
    DPRINTF(MinorExecute, "set custom ctrlVec %i from %i to val %i\n", idx,controlVec[idx],val);
    controlVec[idx]=val;
}

//check if index=1 (0 repre writing)
bool 
CustomControl::ckInfo(int* info)
{
    bool returnVal = true;
    int ist=info[IST];
    if ( ist == VLOAD ) return returnVal;
    for (int i = IDX1; i < IDX1 + numOfIdx(info); i++)
    {
        int isval = DONE;
        int index = info[i];
        if (!ckVal(index,isval))
        {
            returnVal = false;
            break;
        }
    }
    return returnVal;
}

bool
CustomControl::checkCanIss(MinorDynInstPtr inst)
{
    assert(inst->isCustom());
    DPRINTF(MinorExecute, "check if ins PC %s custom ready.\n", *inst);
    int* info = getInfo(inst);
    bool canIss = ckInfo(info);
    if(canIss)  
    {
        if(info[IST]==AAMUL12)
        {
            setVal(TMPA,BUSY);
        }
        else if(info[IST]==AAMUL21)
        {
            setVal(TMPB,BUSY);
        }
        else
        {
            setVal(info[IDX1],BUSY);
        }
        DPRINTF(MinorExecute, "Yes,custom ready\n");
    }    
    delete[] info;
    return canIss;
}

void
CustomControl::doneInsts(MinorDynInstPtr completed_inst)
{
    assert(completed_inst->isCustom());
    int* info = getInfo(completed_inst);

    if(info[IST]==AAMUL12)
    {
        setVal(TMPA,DONE);
    }
    else if(info[IST]==AAMUL21)
    {
        setVal(TMPB,DONE);
    }
    else
    {
        setVal(info[IDX1],DONE);
    }
    delete[] info;
}


} // namespace minor

} // namespace gem5
