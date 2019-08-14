#ifndef STEP_AUTOMAT_H
#define STEP_AUTOMAT_H

#include <iostream>
#include <map>
#include <mutex>
#include <condition_variable>
#include "Sequence.h"
#include "Typelist.h"


#define STACK_SIZE_50 50


const int PRIOR_STORAGE = 0;
const int MINOR_STORAGE = 1;


/*!
    \brief ������� ��� ������ ��������
    ����� ������� (������� FIFO) ������ ��������� ��� ������:
    getState
    setState;
    ��� ������� ������� ��������� ������������ "state::noop"
    � �������� ��������� ���������� ������������
    \tparam class state - ����� ����� ���������
    (
        ��� ��������� 11 enum class
        ��� ���������� ���� ���������� enum class
        � ������ enum_class.h
    )
    \tparam queue_size - ������ �������
    \warning ������������ ��������� enum class �������� "noop"
    \todo ������������ �� �������� ����� ������� � ����� ������������ ��������� getState � setState
*/

template <class state, int queue_size = 1>
class StateQueue
{
private:
    int stateInQueue_;
    state states_[queue_size];
public:
    StateQueue();
    /*!
     \brief �������� ��� ��������� �� ������� �������� � �������

     \param[out] _stateType - �������� ��� �������� ���� ���������
     \return ������ ��������:
      true - ���� ��������� ����
      false - ���� ������� ��������� ������ � �������� �� ������� ������
     */
    bool getState(state &_stateType);

    /*!
     \brief ��������� ��������� ��������� � ������� �������� � �������
     \param[in] _stateType - ��� ���������
     \return void:
     */
    void setState(state _stateType);
};

template<typename state, int queue_size>
StateQueue<state, queue_size>::StateQueue():stateInQueue_(0)
{
    for (int i = 0; i < queue_size; ++i)
        states_[i] = state::noop;
}



template<typename state, int queue_size>
bool StateQueue<state, queue_size>::getState(state &_stateType)
{
    bool result = false;
    if (stateInQueue_ > 0)
    {
        _stateType = states_[stateInQueue_ - 1];
        states_[stateInQueue_ - 1] = state::noop;
        --stateInQueue_;
        result = (_stateType != state::noop);
    }
    return result;
}

template<typename state, int queue_size>
void StateQueue<state, queue_size>::setState(state _stateType)
{
    if (stateInQueue_ > 0)
        if (static_cast<int>(_stateType) == static_cast<int>(states_[0]))
            return;
    if (_stateType == state::noop)
        return;
    if (stateInQueue_ < queue_size)
        ++stateInQueue_;
    if (queue_size > 1)
    {
        for (int i = (stateInQueue_ - 1); i > 0; --i)
        {
            states_[i] = states_[i - 1];
        }
    }
    states_[0] = _stateType;
}

/*!
    \brief ������� ��� ������ ��������
    ����� ������� (������� FILO) ������ ��������� ��� ������:
    getState
    setState;
    ��� ������� ������� ��������� ������������ "state::noop"
    � �������� ��������� ���������� ������������
    \tparam class state - ����� ����� ���������
    (
        ��� ��������� 11 enum class
        ��� ���������� ���� ���������� enum class
        � ������ enum_class.h
    )
    \tparam queue_size - ������ �������
    \warning ������������ ��������� enum class �������� "noop"
    \todo ������������ �� �������� ����� ������� � ����� ������������ ��������� getState � setState
*/
template <class state, int stack_size = 1>
class StateStack
{
private:
    int stateInStack_;
    state states_[stack_size];
public:
    StateStack();

    /*!
     \brief �������� ��� ��������� �� ������� �������� � �������

     \param[out] _stateType - �������� ��� �������� ���� ���������
     \return ������ ��������:
      true - ���� ��������� ����
      false - ���� ������� ��������� ������ � �������� �� ������� ������
     */
    bool getState(state &_stateType);

    /*!
     \brief ��������� ��������� ��������� � ������� �������� � �������
     \param[in] _stateType - ��� ���������
     \return void:
     */
    void setState(state _state);
};

template<typename state, int stack_size>
StateStack<state, stack_size>::StateStack():stateInStack_(0)
{
    for (int i = 0; i < stack_size; ++i)
        states_[i] = state::noop;
}

template<typename state, int stack_size>
bool StateStack<state, stack_size>::getState(state &_stateType)
{
    bool result = false;
    if (stateInStack_ > 0)
    {
        _stateType = states_[0];
        for (int i = 0; i < stateInStack_ - 1; ++i)
        {
            states_[i] = states_[i + 1];
        }
        states_[stateInStack_ - 1] = state::noop;
        --stateInStack_;
        result = (_stateType != state::noop);
    }
    return result;
}

template<typename state, int stack_size>
void StateStack<state, stack_size>::setState(state _state)
{
    if (stateInStack_ > 0)
        if (static_cast<int>(_state) == static_cast<int>(states_[0]))
            return;
    if (stateInStack_ < stack_size)
        ++stateInStack_;
    if (stack_size > 1)
    {
        for (int i = (stateInStack_ - 1); i > 0; --i)
        {
            states_[i] = states_[i - 1];
        }
    }
    states_[0] = _state;
}


/*!
 \brief ������� ����������� ����� ��� ������ ��������.
 ������������ ��� �������� ����������� � ���� ��������� ��������� � ��������� ����������� ����������.
 ������ ����� �� �����, ���� ������������ �������� �++ 11 � ����

*/
template<typename R>
class FuncBase
{
public:
    virtual ~FuncBase(){}

    /*!
     \brief ��������� �� ���������� �������
     */
    virtual R Run() = 0;
};

template <typename R, typename F, class Sequence>
class Func;

/*!
 \brief ����� ��� �������� �������� ��� ����������
 \tparam R - ������������ �������� ���������
 \tparam F - �������
*/
template <typename R, typename F>
class Func<R, F, common::Seq<> >: public FuncBase<R>
{
private:
    /*!
     \brief ����������� ����������� - ��������� ����� �����.
    */
    Func(const Func&);

    /*!
     \brief �������� ������������ - ��������� ����� �����.
    */
    Func& operator =(const Func&);
public:

    /*!
     \brief ����������� ��� �������� ��������
     \tparam U - ������������ ��� ��������� ����������� �����
    */
    template <class U>
    Func (const U & func):func_(func){}
    R Run(){return func_();}
    ~Func(){}
private:
    F * func_;
};

/*!
 \brief ����� ��� �������� �������� ��� ������ ���������
 \tparam R - ������������ �������� ���������
 \tparam F - �������
 \tparam Arg1 - �������� ��������
*/
template <typename R, typename F, class Arg1>
class Func<R, F, common::Seq<Arg1> >: public FuncBase<R>
{
private:
    Func(const Func&);
    Func& operator =(const Func&);
public:
    template <class U, class A1>
    Func (const U & func, const A1 & arg1):func_(func), arg1_(arg1){}

    /*!
     \brief ��������� �� ���������� �������
     \param arg1_ �����-�� ��������
     */
    R Run(){return func_(arg1_);}
    ~Func(){}
private:
    F * func_;
    Arg1 arg1_;
};


/*!
 \brief ����� ��� �������� �������� ��� ������ ���������
 \tparam R - ������������ �������� ���������
 \tparam F - �������
 \tparam Arg1 - �������� ��������
 \tparam Arg2 - �������� ��������
*/
template <typename R, typename F, class Arg1, class Arg2>
class Func<R, F, common::Seq<Arg1, Arg2> >: public FuncBase<R>
{
private:
    Func(const Func&);
    Func& operator =(const Func&);
public:
    template <class U, class A1, class A2>
    Func (const U & func, const A1 & arg1, const A2 & arg2) :
        func_(func), arg1_(arg1), arg2_(arg2)
    {}
    /*!
     \brief ��������� �� ���������� �������
     \param arg1_ �����-�� ��������
     \param arg2_ �����-�� ��������
     */
    R Run()
    {
        return func_(arg1_, arg2_);
    }

    ~Func(){}
private:
    F * func_;
    Arg1 arg1_;
    Arg2 arg2_;
};

/*!
 \brief ����� ��� �������� �������� ��� ������ ���������
 \tparam R - ������������ �������� ���������
 \tparam F - �������
 \tparam Arg1 - �������� ��������
 \tparam Arg2 - �������� ��������
 \tparam Arg3 - �������� ��������
*/
template <typename R, typename F, class Arg1, class Arg2, class Arg3>
class Func<R, F, common::Seq<Arg1, Arg2, Arg3> >: public FuncBase<R>
{
private:
    Func(const Func&);
    Func& operator =(const Func&);
public:
    template <class U, class A1, class A2, class A3>
    Func (const U & func, const A1 & arg1, const A2 & arg2, const A3 & arg3) :
        func_(func), arg1_(arg1), arg2_(arg2), arg3_(arg3)
    {}
    /*!
     \brief ��������� �� ���������� �������
     \param arg1_ �����-�� ��������
     \param arg2_ �����-�� ��������
     \param arg3_ �����-�� ��������
     */
    R Run()
    {
        return func_(arg1_, arg2_, arg3_);
    }

    ~Func(){}
private:
    F * func_;
    Arg1 arg1_;
    Arg2 arg2_;
    Arg2 arg3_;
};

/*!
 \brief ������ ������ ������� ��� ����������, � ����� � �.�. �����������
*/
template<typename R, typename F>
FuncBase<R> *createFunc(const F &func)
{
    return new Func<R, F, common::Seq<> >(func);
}

template<typename R, typename F, class Arg1>
FuncBase<R> *createFunc(const F &func, const Arg1 &arg1)
{
    return new Func<R, F, common::Seq<Arg1> >(func, arg1);
}

template<typename R, typename F, class Arg1, class Arg2>
FuncBase<R> *createFunc(const F &func, const Arg1 &arg1, const Arg2 &arg2)
{
    return new Func<R, F, common::Seq<Arg1, Arg2> >(func, arg1, arg2);
}

template<typename R, typename F, class Arg1, class Arg2, class Arg3>
FuncBase<R> *createFunc(const F &func, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
{
    return new Func<R, F, common::Seq<Arg1, Arg2, Arg3> >(func, arg1, arg2, arg3);
}



/*!
    \brief ����� ��������
    \tparam T - ����� �������� �������, ������ �������� ������� ����������������
    \tparam S - ����� ������������ �������
    \tparam state - ����� ��������� (�� ����� enum_class)
    \param AutomatList - ��������� ��� ������ ��������� + ������� (��������, ������� ������ ����������� ��� ���� ���������)
    \param prior_command - ������� ������������ ������
    \param minor_command - ������� �������������� ������
    \param obj - ������, � ������� �������� �������
    \param canTryPushState_ - ������ ����������, ������� ������������ ��� ��������,
    ������������� �������� ������ � ���������� ���������
    \warning �������������� mutex �������� ������ ��� 11 ���������. ��� ������ � 3 ����������, ����� ������������ ���� ����������
    \warning �������������� condition_variable �������� ������ ��� 11 ���������. ��� ������ � 3 ����������, ����� ������������ ���� ����������
    \todo ������ �� ���������� ��������� S. ������� ��� ������ ������ � ������������ �� ���������� ������.
    \todo �������� ����������� �������� ������ �������� � �������� ������� ����� �����������
*/
template <typename T, template <typename, int> class S, typename state>
class Automat
{
private:
    std::map<state, FuncBase<state> *> AutomatList;
    S<state, STACK_SIZE_50> prior_command;
    S<state, STACK_SIZE_50> minor_command;
    T * obj;
    std::mutex m_;
    std::condition_variable	cond_;

public:
    Automat(): canTryPushState_(true){m_.unlock();}
    virtual ~Automat(){}

    /*!
     \brief ������������� ������� ��� ���������� ������ ��������� � ��������
     � ��������� ����������� ����������
    */
    template<typename F>
    void add(state _state, const F &func)
    {
        AutomatList.insert(std::make_pair(_state, createFunc<state>(func)));
    }

    template<typename F, class Arg1>
    void add(state _state, const F &func, const Arg1 &arg1)
    {
        AutomatList.insert(std::make_pair(_state, createFunc<state>(func, arg1)));
    }

    template<typename F, class Arg1, class Arg2>
    void add(state _state, const F &func, const Arg1 &arg1, const Arg2 &arg2)
    {
        AutomatList.insert(std::make_pair(_state, createFunc<state>(func, arg1, arg2)));
    }

    template<typename F, class Arg1, class Arg2, class Arg3>
    void add(state _state, const F &func, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
    {
        AutomatList.insert(std::make_pair(_state, createFunc<state>(func, arg1, arg2, arg3)));
    }

    /*!
     \brief ���������� ��������� � ������� �����������
     \param _stateType ���������
     \param _stackType ��� ������� (������������ ��� �� ������������)
    */
    void pushState(state _stateType, const int _stackType);

    /*!
     \brief ���������� ��������� � ���������� �������, ������� ��� ����� � ������� ����������
    */
    void exec();

    /*!
     \brief �������� � ������� ���������� ��������� � ��������� ����������,
     ����������� ����� ������� �������� �������� �������.
     \warning ����������� ����� �� ����������� �� ���������, ������� ������ ��� ���� ��������.
     \param _stateType ���������
     \param _stackType ��� ������� (������������ ��� �� ������������)
    */
    void exec(state _stateType, const int _stackType);
private:

    bool canTryPushState_;

    /*!
      \brief ������� ��� ��������
      \return ������ ��������, ����������� �� ��, ����� �� ����������� ������� ���������
      \warning � 11 ��������� ����� ������������ ������
     */
    bool canTryPushState() const {return canTryPushState_;}
    typedef Automat<T, S, state> automat;

    class canPushState
	{
	public:
		canPushState(const automat& _a) : a_(_a) {}

		bool operator()() const { return a_.canTryPushState(); }
	private:
		const automat& a_;
	};

    /*!
      \brief ������� ��� ��������
      \return ������ ��������, ����������� �� ��, ����� �� ����������� ���������� ���������
      \warning � 11 ��������� ����� ������������ ������
     */
    class canExec // ������� ��� ��������� (� 11 ������ ����� ������)
	{
	public:
		canExec(const automat& _a) : a_(_a) {}

		bool operator()() const { return !a_.canTryPushState(); }
	private:
		const automat& a_;
	};
};

template <typename T, template <typename, int> class S, typename state>
void Automat<T, S, state>::pushState(state _stateType, const int _stackType)
{
    std::unique_lock<std::mutex> lock(m_);
    canTryPushState_ = true;
    /*!
     \brief ��� ����� �������� �������� � �������� ����������
    */
    cond_.wait(lock, canPushState(*this));
    switch (_stackType)
    {
        case PRIOR_STORAGE:
        {
            /*!
             \brief ���������� ��������� � ������������ �������
            */
            prior_command.setState(_stateType);
            break;
        }
        case MINOR_STORAGE:
        {
            /*!
             \brief ���������� ��������� � �������� �������
            */
            minor_command.setState(_stateType);
            break;
        }
    }
    canTryPushState_ = false;
    cond_.notify_all();
}

template <typename T, template <typename, int> class S, typename state>
void Automat<T, S, state>::exec(state _stateType, const int _stackType)
{
    pushState(_stateType, _stackType);
    exec();
}

template <typename T, template <typename, int> class S, typename state>
void Automat<T, S, state>::exec()
{
    std::unique_lock<std::mutex> lock(m_);
    canTryPushState_ = false;
    /*!
     \brief ��� ����� �������� �������� � �������� ���������� ���������
    */
    cond_.wait(lock, canExec(*this));
    state _stateType;
    /*!
     \brief ���� ���� ���-�� � ������������ �������
    */
    if (prior_command.getState(_stateType))
    {
        /*!
         \brief ��������� � ������������ �������� ���������,
         ������� ���� �������� � ���������� ���������� ��������
        */
        prior_command.setState(AutomatList[_stateType]->Run());
    }
    /*!
     \brief ���� ���� ���-�� � �������� �������
    */
    else if (minor_command.getState(_stateType))
    {
        /*!
         \brief ��������� � ������������ �������� ���������,
         ������� ���� �������� � ���������� ���������� ��������
        */
        minor_command.setState(AutomatList[_stateType]->Run());
    };

    canTryPushState_ = true;
    cond_.notify_all();
}



#endif // AUTOMAT_H


