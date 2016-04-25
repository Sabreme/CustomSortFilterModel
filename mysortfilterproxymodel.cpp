#include <QtGui>

#include "mysortfilterproxymodel.h"

MySortFilterProxyModel::MySortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

void MySortFilterProxyModel::setFilterMinimumDate(const QDate &date)
{
    minDate = date;
    invalidateFilter();
}

void MySortFilterProxyModel::setFilterMaximumDate(const QDate &date)
{
    maxDate = date;
    invalidateFilter();
}

bool MySortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex index1 = sourceModel()->index(sourceRow, 1, sourceParent);
    QModelIndex index2 = sourceModel()->index(sourceRow, 2, sourceParent);

    return (sourceModel()->data(index0).toString().contains(filterRegExp())
            || sourceModel()->data(index1).toString().contains(filterRegExp()))
            && dateInRange(sourceModel()->data(index2).toDate());
}

bool MySortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
     QVariant leftData = sourceModel()->data(left);
     QVariant rightData = sourceModel()->data(right);

     if (leftData.type() == QVariant::DateTime)
     {
         return leftData.toDateTime() < rightData.toDateTime();
     }
     else
     {
         QRegExp *emailPattern = new QRegExp("([\\w\\.]*@[\\w\\.]*)");

         QString leftString = leftData.toString();
         if(left.column() == 1 && emailPattern->indexIn(leftString) != -1)
             leftString = emailPattern->cap(1);

         QString rightString = rightData.toString();
         if(right.column() == 1 && emailPattern->indexIn(rightString) != -1)
             rightString = emailPattern->cap(1);

         return QString::localeAwareCompare(leftString, rightString) < 0;
     }
}

bool MySortFilterProxyModel::dateInRange(const QDate &date) const
{

    return (!minDate.isValid() || date > minDate)
           && (!maxDate.isValid() || date < maxDate);
}

