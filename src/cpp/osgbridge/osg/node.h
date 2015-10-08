#pragma once
#include <QtCore>
#include "object.h"
#include <osg/Node>

class Node : public Object
{
    Q_OBJECT

public:
    Node();
    void classBegin() override;

private:
    osg::Node* node;
};
