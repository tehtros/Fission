/*
RefCounted.h
Theodore DeRego
Copyright 2012

The abstract base class for all objects with reference counting
*/

#ifndef REFCOUNTED_H
#define REFCOUNTED_H

class RefCounted
{
    public:
        RefCounted(){mRefs=1;}
        virtual ~RefCounted(){}

        void addRef(){mRefs++;}
        void release()
        {
            mRefs--;
            if (mRefs == 0)
                delete this;
        }

    protected:
    private:
        int mRefs;
};

#endif // REFCOUNTED_H
