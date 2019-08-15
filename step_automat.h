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
    \brief очередь для работы автомата
    класс очереди (очередь FIFO) должен содержать два метода:
    getState
    setState;
    Для задания пустого состояния используется "state::noop"
    в качестве шаблонных параметров используется
    \tparam class state - класс типов состояния
    (
        для стандарта 11 enum class
        для стандартов ниже реализован enum class
        в модуле enum_class.h
    )
    \tparam queue_size - размер очереди
    \warning обязательным значением enum class является "noop"
    \todo унаследовать от базового класа очереди с чисто виртуальными функциями getState и setState
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
     \brief получает тип состояния из очереди согласно её политки

     \param[out] _stateType - пораметр для возврата типа состояния
     \return булево значение:
      true - если состояние есть
      false - если очередь состояний пустая и получить не удалось ничего
     */
    bool getState(state &_stateType);

    /*!
     \brief добавляет следующее состояние в очередь согласно её политки
     \param[in] _stateType - тип состояния
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
    \brief очередь для работы автомата
    класс очереди (очередь FILO) должен содержать два метода:
    getState
    setState;
    Для задания пустого состояния используется "state::noop"
    в качестве шаблонных параметров используется
    \tparam class state - класс типов состояния
    (
        для стандарта 11 enum class
        для стандартов ниже реализован enum class
        в модуле enum_class.h
    )
    \tparam queue_size - размер очереди
    \warning обязательным значением enum class является "noop"
    \todo унаследовать от базового класа очереди с чисто виртуальными функциями getState и setState
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
     \brief получает тип состояния из очереди согласно её политки

     \param[out] _stateType - пораметр для возврата типа состояния
     \return булево значение:
      true - если состояние есть
      false - если очередь состояний пустая и получить не удалось ничего
     */
    bool getState(state &_stateType);

    /*!
     \brief добавляет следующее состояние в очередь согласно её политки
     \param[in] _stateType - тип состояния
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
 \brief базовый виртуальный класс для класса функтора.
 используется для создания привязанных к типу состояния функторов с различным количеством параметров.
 Данный класс не нужен, если использовать стандарт С++ 11 и выше

*/
template<typename R>
class FuncBase
{
public:
    virtual ~FuncBase(){}

    /*!
     \brief реализация доджна запускать на выполнение функтор
     */
    virtual R Run() = 0;
};

template <typename R, typename F, class Sequence>
class Func;

/*!
 \brief класс для создания функтора без параметров
 \tparam R - возвращаемое значение функотора
 \tparam F - функтор
*/
template <typename R, typename F>
class Func<R, F, common::Seq<> >: public FuncBase<R>
{
private:
    Func(const Func&);
    Func& operator =(const Func&);
public:

    /*!
     \brief конструктор для создания функтора
     \tparam U - предусмотрен для возможной конвертации типов
    */
    template <class U>
    Func (const U & func):func_(func){}
    R Run(){return func_();}
    ~Func(){}
private:
    F * func_;
};

/*!
 \brief класс для создания функтора для одного параметра
 \tparam R - возвращаемое значение функотора
 \tparam F - функтор
 \tparam Arg1 - параметр функтора
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
     \brief запускает на выполнение функтор
     \param arg1_ какой-то параметр
     */
    R Run(){return func_(arg1_);}
    ~Func(){}
private:
    F * func_;
    Arg1 arg1_;
};


/*!
 \brief класс для создания функтора для одного параметра
 \tparam R - возвращаемое значение функотора
 \tparam F - функтор
 \tparam Arg1 - параметр функтора
 \tparam Arg2 - параметр функтора
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
     \brief запускает на выполнение функтор
     \param arg1_ какой-то параметр
     \param arg2_ какой-то параметр
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
 \brief класс для создания функтора для одного параметра
 \tparam R - возвращаемое значение функотора
 \tparam F - функтор
 \tparam Arg1 - параметр функтора
 \tparam Arg2 - параметр функтора
 \tparam Arg3 - параметр функтора
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
     \brief запускает на выполнение функтор
     \param arg1_ какой-то параметр
     \param arg2_ какой-то параметр
     \param arg3_ какой-то параметр
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
 \brief создаёт объект функтор без параметров, с одним и т.д. параметрами
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
    \brief класс автомата
    \tparam T - класс целевого объекта, работу которого следует автоматизировать
    \tparam S - класс используемой очереди
    \tparam state - класс состояний (по факту enum_class)
    \param AutomatList - хранилище для связок состояние + функтор (действие, которое должно выполниться при этом состоянии)
    \param prior_command - очередь приоритетных команд
    \param minor_command - очередь НЕприоритетных команд
    \param obj - объект, с которым работает автомат
    \param canTryPushState_ - булева переменная, которая используется для кондвара,
    разруливающая операции втавки и выполнения состояний
    \warning использованный mutex работает только для 11 стандарта. При работе с 3 стандартом, стоит использовать свою реализацию
    \warning использованный condition_variable работает только для 11 стандарта. При работе с 3 стандартом, стоит использовать свою реализацию
    \todo убрать из шаблонного параметра S. Сделать его членом класса и унаследовать от шаблонного класса.
    \todo добавить возможность задавать размер минорной и мажорной очереди через конструктор
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
     \brief перегруженные функции для добавления связки состояния и функтора
     с различным количеством параметров
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
     \brief добавление состояния в очередь выполненияв
     \param _stateType состояние
     \param _stackType тип очереди (приоритетная или не приоритетная)
    */
    void pushState(state _stateType, const int _stackType);

    /*!
     \brief выполнение связанной с состоянием функции, которая уже лежит в очереди выполнения
    */
    void exec();

    /*!
     \brief добавить в очередь выполнения состояние и запустить выполнятор,
     выполняться будет функция согласно политике очереди.
     \warning выполняться будет не обязательно то состояние, которое только что было заведено.
     \param _stateType состояние
     \param _stackType тип очереди (приоритетная или не приоритетная)
    */
    void exec(state _stateType, const int _stackType);
private:

    bool canTryPushState_;

    /*!
      \brief функтор для кондвара
      \return булево значение, указывающее на то, можно ли производить вставку состояния
      \warning в 11 стандарте можно использовать лямбду
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
      \brief функтор для кондвара
      \return булево значение, указывающее на то, можно ли производить выполнение функции состояния
      \warning в 11 стандарте можно использовать лямбду
     */
    class canExec // функтор для кондвара (в 11 плюсах можно лямбду)
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
     \brief ждём когда закончат работать с очередью выполнятор
    */
    cond_.wait(lock, canPushState(*this));
    switch (_stackType)
    {
        case PRIOR_STORAGE:
        {
            /*!
             \brief добавление состояния в приоритетную очередь
            */
            prior_command.setState(_stateType);
            break;
        }
        case MINOR_STORAGE:
        {
            /*!
             \brief добавление состояния в минорную очередь
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
     \brief ждём когда закончит работать с очередью добавлятор состояний
    */
    cond_.wait(lock, canExec(*this));
    state _stateType;
    /*!
     \brief если есть что-то в приоритетной очереди
    */
    if (prior_command.getState(_stateType))
    {
        /*!
         \brief выполнить и одновременно положить состояние,
         которое было получено в результате выполнения функтора
        */
        prior_command.setState(AutomatList[_stateType]->Run());
    }
    /*!
     \brief если есть что-то в минорной очереди
    */
    else if (minor_command.getState(_stateType))
    {
        /*!
         \brief выполнить и одновременно положить состояние,
         которое было получено в результате выполнения функтора
        */
        minor_command.setState(AutomatList[_stateType]->Run());
    };

    canTryPushState_ = true;
    cond_.notify_all();
}



#endif // AUTOMAT_H


