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
        RefCounted();
        virtual ~RefCounted();
    protected:
        int mRefs;
    private:
};

#endif // REFCOUNTED_H
