#ifndef ENUM_CLASS_H
#define ENUMC_LASS_H


// Енум класс. В 11 плюсах уже есть енум класс, это для 03 плюсиков.

//class rr_steps
//{
//public:
//  enum state
//      {
//          GetInfoMRLS = 0,
//          SendInfoMRLS,
//          GetInfoBOU,
//          SendInfoBOU,
//          error,
//          noop
//      };
//
//  rr_steps() : val_(noop) {}
//  rr_steps(state st) : val_(st) {}
//  rr_steps(const rr_steps& rhs) : val_(rhs.val_) {}
//
//  operator state() const { return val_; }
//  operator int() const { return val_; }
//
//  rr_steps operator=(const rr_steps& rhs)
//  {
//      val_ = rhs.val_;
//      return *this;
//  }
//
//  rr_steps operator=(state val)
//  {
//      val_ = val;
//      return *this;
//  }
//
//  bool operator<(const rr_steps& lhs) const { return val_ < lhs.val_; }
//private:
//  state val_;
//};

























// пример использования
//BEGIN_STATE_CLASS(rr_steps)
//    rrs_Response = 0,
//    rrs_GetInf,
//	rrs_GetConst,
//	rrs_ClearFlash,
//    rrs_CorrectingData,
//	rrs_error
//END_STATE_CLASS(rr_steps);


#define BEGIN_STATE_CLASS(name) \
    class name                  \
    {                           \
    public:                     \
        enum Type               \
        {

#define END_STATE_CLASS(name)   \
            ,noop               \
        };                      \
                                \
        name() : val_(noop) {}  \
        name(Type st) : val_(st) {} \
        name(const name& rhs) : val_(rhs.val_) {} \
        \
        operator Type() const { return val_; } \
        operator int() const { return val_; } \
        \
        name operator=(const name& rhs) \
        { \
            val_ = rhs.val_; \
            return *this;\
        }\
        \
        name operator=(Type val)\
        {\
            val_ = val;\
            return *this;\
        }\
        \
        bool operator<(const name& lhs) const { return val_ < lhs.val_; } \
    private: \
        Type val_; \
    };


#endif /* ENUMCLASS_H_ */
