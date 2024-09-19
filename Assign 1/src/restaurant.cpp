#include "main.h"
class imp_res : public Restaurant{
public:
    class TimeOfCus{
    public:
        customer *value;
        TimeOfCus *next=nullptr;
        TimeOfCus *prev=nullptr;
        bool InTable;
    public:
        TimeOfCus(customer *value, bool InTable){
            this->value=value;
            this->InTable=InTable;
        }
        ~TimeOfCus(){ delete value; }
    };
private:
    customer *Table = nullptr;
    int sizeTable = 0;
    customer *Waiting = nullptr;
    int sizeWaiting = 0;
    TimeOfCus *CusFirst = nullptr;
    TimeOfCus *CusLast = nullptr;
    int energyThuatSu = 0;
    int energyOanLinh = 0;
public:
    bool checkTable(customer* cus){
        if (cus->energy==0 || sizeTable + sizeWaiting >= MAXSIZE * 2) return true;
            TimeOfCus *temp = CusFirst;
            for (int i = 0; i < sizeTable + sizeWaiting; i++){
                if (temp->value->name == cus->name)
                    return true;
                temp = temp->next;
            }
        return false;
    }
    bool addCusToWaiting(customer* cus){
        if (sizeTable == MAXSIZE && sizeWaiting == 0){
            Waiting = cus;
            Waiting->next = Waiting->prev = Waiting;
            TimeOfCus *newTimeOfCus = new TimeOfCus(cus, false);
            newTimeOfCus->prev = CusLast;
            CusLast->next = newTimeOfCus;
            CusLast = CusLast->next;
            sizeWaiting++;
            return true;
         }else if(sizeTable == MAXSIZE){
            cus->next = Waiting;
            cus->prev = Waiting->prev;
            Waiting->prev = cus;
            cus->prev->next = cus;
            TimeOfCus *newTimeOfCus = new TimeOfCus(cus, false);
            newTimeOfCus->prev = CusLast;
            CusLast->next = newTimeOfCus;
            CusLast = CusLast->next;
            sizeWaiting++;
            return true;
        }
        return false;
    }
    void addCusToTable(customer* cus){
        if (sizeTable == 0){
            Table = cus;
            Table->next = Table->prev = Table;
            CusLast = CusFirst = new TimeOfCus(Table, true);
            sizeTable++;
            return;
        }
        if (sizeTable >= MAXSIZE / 2){
            customer *temp = Table;
            for (int i = 0; i < sizeTable; i++){
                int energyTemp = abs(cus->energy - temp->energy);
                int energyTable = abs(cus->energy - Table->energy);
                if (energyTemp > energyTable) Table = temp;
                temp = temp->next;
            }
        }
        if (cus->energy < Table->energy){
            cus->prev = Table->prev;
            cus->next = Table;
            Table->prev = cus;
            cus->prev->next = cus;
        }else{
            cus->prev = Table;
            cus->next = Table->next;
            Table->next = cus;
            cus->next->prev = cus;
        }
        Table = cus;
        TimeOfCus *newTimeOfCus = new TimeOfCus(Table, true);
        newTimeOfCus->prev = CusLast;
        CusLast->next = newTimeOfCus;
        CusLast = CusLast->next;
        sizeTable++;
        return;
    }

    void RED(string name, int energy){
        // cout << name << " " << energy << endl;
        customer *cus = new customer(name, energy, nullptr, nullptr);
        //cout << MAXSIZE << endl;
        if(checkTable(cus)){
delete cus;
return;
        } 
        if(addCusToWaiting(cus)==true) return;
        addCusToTable(cus);
    }

    void checkTimeOfCus(TimeOfCus *temp){
        if (sizeWaiting + sizeTable == 1) CusFirst = CusLast = nullptr;
        else if (temp == CusFirst || temp == CusLast){
            if (temp == CusLast){
                CusLast = CusLast->prev;
                CusLast->next = nullptr;
            }else{
                CusFirst = CusFirst->next;
                CusFirst->prev = nullptr;
            }
        }else{
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
        }
    }
    void RemoveTimeOfCus(int num){
        while (num > 0 && sizeTable != 0){
            TimeOfCus *temp = CusFirst;
            for (int i = 0; i < sizeWaiting + sizeTable; i++){
                if (temp->InTable) break;
                temp = temp->next;
            }
            checkTimeOfCus(temp);
            if (temp->InTable == true) RemoveTimeOfCusIntable(temp);
            else RemoveTimeOfCusWaiting(temp);
            num--;
        }
    }
    void RemoveTimeOfCusIntable(TimeOfCus *temp){
        if (sizeTable == 1){
            sizeTable = 0;
            Table = nullptr;
        }else{
            temp->value->next->prev = temp->value->prev;
            temp->value->prev->next = temp->value->next;
            if (temp->value->energy < 0) Table = temp->value->prev;
            else Table = temp->value->next;
            sizeTable--;
        }
        delete temp;
    }
    void RemoveTimeOfCusWaiting(TimeOfCus *temp){
        if (sizeWaiting == 1){
            sizeWaiting = 0;
            Waiting = nullptr;
        }else{
            if (temp->value == Waiting)
                Waiting = Waiting->next;
            temp->value->prev->next = temp->value->next;
            temp->value->next->prev = temp->value->prev;
            sizeWaiting--;
        }
        delete temp;
    }
    void addWaitingToTable(){
        while (sizeWaiting && sizeTable != MAXSIZE){
            TimeOfCus *temp = CusFirst;
            customer *cus = Waiting;
            for (int i = 0; i < sizeWaiting + sizeTable; i++){
                if (temp->value == Waiting){
                    temp->InTable = true;
                    break;
                }
                temp = temp->next;
            }
            if (sizeWaiting == 1) Waiting = nullptr;
            else{
                Waiting = cus->next;
                cus->prev->next = Waiting;
                Waiting->prev = cus->prev;
            }
            sizeWaiting--;
            if (sizeTable == 0){
                Table = cus;
                cus->next = cus->prev = cus;
            }
            else{
                if (sizeTable >= MAXSIZE / 2){
                    customer *temp = Table;
                    for (int i = 0; i < sizeTable; i++){
                        int energyTemp = abs(cus->energy - temp->energy);
                        int energyTable = abs(cus->energy - Table->energy);
                        if (energyTemp > energyTable) Table = temp;
                        temp = temp->next;
                    }
                }
                if (cus->energy >= Table->energy){
                    cus->next = Table->next;
                    cus->prev = Table;
                    Table->next = cus;
                    cus->next->prev = cus;
                }else{
                    cus->next = Table;
                    cus->prev = Table->prev;
                    Table->prev = cus;
                    cus->prev->next = cus;
                }
            }
            Table = cus;
            sizeTable++;
        }
    }

    void BLUE(int num){
        // cout << "blue "<< num << endl;
        if (sizeTable == 0) return;
        RemoveTimeOfCus(num);
        addWaitingToTable();
    }

    bool checkCus(customer *a, customer *b){
        TimeOfCus *temp = CusLast;
        while (temp != NULL){
            if (temp->value == b) return false;
            else if (temp->value == a) break;
            temp = temp->prev;
        }
        return true;
    }
    customer *getCus(customer *cus, int index){
        for (int i = 0; i < index; i++)  cus = cus->next;
        return cus;
    }
    bool checkBreakInssort(customer* x,customer* y){
        if (abs(x->energy) < abs(y->energy) || 
                    (abs(x->energy) == abs(y->energy) && checkCus(x, y)))
            return true;
        return false;
    }
    int inssort2(customer *head, int n, int incr){
        int numberswap = 0;
        int i = incr;
        while (i < n){
            int j = i;
            while (j >= incr){
                customer *y = getCus(head, j - incr);
                customer *x = getCus(head, j);
                if (checkBreakInssort(x,y)) break;
                customer *temp = new customer();
                temp->prev = x->prev;
                temp->next = x->next;
                x->prev->next = temp;
                x->next->prev = temp;
                x->prev = y->prev;
                x->next = y->next;
                y->prev->next = x;
                y->next->prev = x;
                y->prev = temp->prev;
                y->next = temp->next;
                temp->next->prev = temp->prev->next = y;
                delete temp;
                numberswap++;
                if (head == y) head = x;
                if (y == Waiting) Waiting = x;
                j -= incr;
            }
            i += incr;
        }
        return numberswap;
    }
    customer* checkMaxEnergyWaiting(){
        TimeOfCus *max = nullptr;
        TimeOfCus *temp = CusLast;
        for (int i = 0; i < sizeWaiting + sizeTable; i++){
            if ((max == nullptr || abs(temp->value->energy) > abs(max->value->energy)) 
                     && temp->InTable == 0){
                max = temp;
            }
            temp = temp->prev;
        }
        return max->value;
    }
    int LengthOfShell(){
        customer* temp=checkMaxEnergyWaiting();
        customer* temp2 = Waiting;
        int n = 1;
        while (temp2 != temp){
            temp2 = temp2->next;
            n++;
        }
        return n;
    }
    void PURPLE(){
        // cout << "purple"<< endl;
        if (sizeWaiting < 2) return;
        int countswap = 0;
        int n=LengthOfShell();
        for (int i = n / 2; i > 2; i /= 2){
            int j = 0;
            while (j < i){
                countswap += inssort2(getCus(Waiting, j), n - j, i);
                j++;
            }
        }
        countswap += inssort2(Waiting, n, 1);
        BLUE(countswap % MAXSIZE);
    }
    void reversalOanLinh(customer* head,customer* tail){
        head = Table;
        for (int i = 0; i < sizeTable; i++){
            if (head->energy < 0) break;
            head = head->prev;
        }
        tail = Table->next;
        for (int i = 0; i < sizeTable; i++){
            if (tail->energy < 0) break;
            tail = tail->next;
        }
        if (head->energy < 0 && head != tail){
            for (int i = 0; i < sizeTable / 2; i++){
                customer *change = new customer();
                change->prev = head->prev;
                change->next = head->next;
                head->prev->next = change;
                head->next->prev = change;
                head->prev = tail->prev;
                head->next = tail->next;
                tail->prev->next = head;
                tail->next->prev = head;
                tail->prev = change->prev;
                tail->next = change->next;
                change->next->prev = change->prev->next = tail;
                delete change;
                customer *temp = head;
                head = tail;
                tail = temp;
                head = head->prev;
                for (int i = 0; i < sizeTable; i++){
                    if (head->energy < 0) break;
                    head = head->prev;
                }
                if (head == tail) break;
                tail = tail->next;
                for (int i = 0; i < sizeTable; i++){
                    if (tail->energy < 0) break;
                    tail = tail->next;
                }
                if (head == tail) break;
            }
        }
    }
    void reversalThuatSu(customer* head,customer* tail,customer* ThuatSu){
        head = ThuatSu;
        for (int i = 0; i < sizeTable; i++){
            if (head->energy > 0) break;
            head = head->prev;
        }
        tail = ThuatSu->next;
        for (int i = 0; i < sizeTable; i++){
            if (tail->energy > 0)break;
            tail = tail->next;
        }
        if (head->energy > 0 && head != tail){
            for (int i = 0; i < sizeTable / 2; i++){
                customer *change = new customer();
                change->prev = head->prev;
                change->next = head->next;
                head->prev->next = change;
                head->next->prev = change;
                head->prev = tail->prev;
                head->next = tail->next;
                tail->prev->next = head;
                tail->next->prev = head;
                tail->prev = change->prev;
                tail->next = change->next;
                change->next->prev = change->prev->next = tail;
                delete change;
                customer *temp = head;
                head = tail;
                tail = temp;
                head = head->prev;
                for (int i = 0; i < sizeTable; i++){
                    if (head->energy > 0) break;
                    head = head->prev;
                }
                if (head == tail) break;
                tail = tail->next;
                for (int i = 0; i < sizeTable; i++){
                    if (tail->energy > 0) break;
                    tail = tail->next;
                }
                if (head == tail)
                    break;
            }
        }
    }

    void REVERSAL(){
        // cout << "reversal" << endl;
        if (sizeTable < 2) return;
        customer *tail = nullptr;
        customer *head = nullptr;
        customer *ThuatSu = Table;
        for (int i = 0; i < sizeTable; i++){
            if (ThuatSu->energy > 0) break;
            ThuatSu = ThuatSu->prev;
        }
        reversalOanLinh(head,tail);
        reversalThuatSu(head,tail,ThuatSu);
    }

    void PrintVoid(customer* head,customer* tail){
        customer *Cusmin = head;
        customer *temp = head->next;
        while (temp != tail->next){
            if (temp->energy < Cusmin->energy)
                Cusmin = temp;
            temp = temp->next;
        }
        temp = Cusmin;
        while (temp != tail){
            temp->print();
            temp = temp->next;
        }
        tail->print();
        temp = head;
        while (temp != Cusmin){
            temp->print();
            temp = temp->next;
        }
    }
    bool checkSubArray(int totalMin,int total,int maxsize,int j){
         if ((totalMin == total && j + 1 >= maxsize || totalMin > total) 
                    && j > 2)
            return true;
        return false;
    }
    void UNLIMITED_VOID(){
        // cout << "unlimited_void" << endl;
        if (sizeTable < 4) return;
        int maxsize = 0;
        customer *tail = nullptr;
        customer *head = nullptr;
        customer *Cusi = Table;
        int totalMin = 1000000000;
        for (int i = 0; i < sizeTable; i++){
            int total = 0;
            customer *Cusj = Cusi;
            for (int j = 0; j < sizeTable; j++){
                total += Cusj->energy;
                if (checkSubArray(totalMin,total,maxsize,j)){       
                    totalMin = total;
                    maxsize = j + 1;
                    tail = Cusj;
                    head = Cusi;
                }
                Cusj = Cusj->next;
            }
            Cusi = Cusi->next;
        }
        PrintVoid(head,tail);
    }

    int SumEnergyThuatSu(){
        TimeOfCus *temp = CusFirst;
        int sum=0;
        while(temp!=nullptr){
            if (temp->value->energy > 0) 
                sum += temp->value->energy;
            temp = temp->next;
        }
        return sum;
    }
    int SumEnergyOanLinh(){
        TimeOfCus *temp = CusFirst;
        int sum=0;
        while(temp!=nullptr){
            if (temp->value->energy < 0) 
                sum += temp->value->energy;
            temp = temp->next;
        }
        return sum;
    }
    void Display(TimeOfCus *head, TimeOfCus *tail){
        if (head != nullptr && head != tail->next){
            Display(head->next, tail);
            head->value->print();
        }
    }
    void RemoveTheWeak(TimeOfCus* ThuatSuHead,TimeOfCus* ThuatSuTail,TimeOfCus* OanLinhHead,TimeOfCus* OanLinhTail,int energyOanLinh, int energyThuatSu){
         if (abs(energyOanLinh) <= energyThuatSu){
            Display(OanLinhHead, OanLinhTail);
            while (OanLinhHead != nullptr){
                TimeOfCus *temp = OanLinhHead;
                OanLinhHead = OanLinhHead->next;
                if (temp->InTable == true) RemoveTimeOfCusIntable(temp);
                else RemoveTimeOfCusWaiting(temp);
            }
            CusLast = ThuatSuTail;
            CusFirst = ThuatSuHead;
        }else{
            Display(ThuatSuHead, ThuatSuTail);
            while (ThuatSuHead != nullptr){
                TimeOfCus *temp = ThuatSuHead;
                ThuatSuHead = ThuatSuHead->next;
                if (temp->InTable == true) RemoveTimeOfCusIntable(temp);
                else RemoveTimeOfCusWaiting(temp);
            }
            CusLast = OanLinhTail;
            CusFirst = OanLinhHead;
        }
    }

    void DOMAIN_EXPANSION(){
        // cout << "domain_expansion" << endl;
        if (sizeTable == 0) return;
        energyThuatSu = SumEnergyThuatSu();
        energyOanLinh = SumEnergyOanLinh();
        TimeOfCus *ThuatSuHead = nullptr;
        TimeOfCus *ThuatSuTail = nullptr;
        TimeOfCus *OanLinhHead = nullptr;
        TimeOfCus *OanLinhTail = nullptr;
        TimeOfCus *tempTimeOfCus = CusFirst;
        for (int i = 0; i < sizeTable + sizeWaiting; i++){
            if (tempTimeOfCus->value->energy > 0){
                if (ThuatSuHead == nullptr){
                    ThuatSuHead = ThuatSuTail = tempTimeOfCus;
                }
                else{
                    tempTimeOfCus->prev = ThuatSuTail;
                    ThuatSuTail->next = tempTimeOfCus;
                    ThuatSuTail = tempTimeOfCus;
                }
            }else if (tempTimeOfCus->value->energy < 0){
                if (OanLinhHead == nullptr){
                    OanLinhHead = OanLinhTail = tempTimeOfCus;
                }
                else{
                    tempTimeOfCus->prev = OanLinhTail;
                    OanLinhTail->next = tempTimeOfCus;
                    OanLinhTail = tempTimeOfCus;
                }
            }
            tempTimeOfCus = tempTimeOfCus->next;
        }
        if (ThuatSuHead != nullptr) ThuatSuHead->prev = ThuatSuTail->next = nullptr;
        if (OanLinhHead != nullptr) OanLinhHead->prev = OanLinhTail->next = nullptr;
        RemoveTheWeak(ThuatSuHead,ThuatSuTail,OanLinhHead,OanLinhTail,energyOanLinh,energyThuatSu);
        addWaitingToTable();
    }

    void PrintWaiting(){
       int count = 0;
       customer *temp = Waiting;
            while (count < sizeWaiting){
                temp->print();
                temp = temp->next;
                count++;
            }
    }
    void PrintTable(int id){
        int count = 0;
        if(id==1){
             customer *temp = Table;
            while (count < sizeTable){
                temp->print();
                temp = temp->next;
                count++;
            }
        }else{
            customer *temp = Table;
            while (count < sizeTable){
                temp->print();
                temp = temp->prev;
                count++;
            }
        }
    }

    void LIGHT(int num){
        // cout << "light " << num << endl; 
        if (num == 0 && sizeWaiting != 0) PrintWaiting();
        else if (num > 0) PrintTable(1);
        else if (num < 0) PrintTable(-1);
    }
    ~imp_res(){
        while (CusLast!=nullptr){
            TimeOfCus *temp = CusLast;
            CusLast = CusLast->prev;
            delete temp;
        }
    }
};