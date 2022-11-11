#pragma once
#include <QLayout>
#include <QRect>
#include <QStyle>
//��Ӧʽ����
//���Ը����ӿؼ��Ĵ�С������Լ����ֵĴ�С�Զ���˳�����пؼ������ҿ���ѡ���󣬾��У�����������ʽ
enum AlignmentStyle {
	Left = 0,
	Right = 1,
	Center,
};
class AutoGridLayout :public QLayout
{
	Q_OBJECT

public:
    explicit AutoGridLayout(QWidget* parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);
    explicit AutoGridLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);
    ~AutoGridLayout() override;

    void addItem(QLayoutItem* item) override;
    int horizontalSpacing() const;
    int verticalSpacing() const;
    Qt::Orientations expandingDirections() const override;
    bool hasHeightForWidth() const override;
    int heightForWidth(int) const override;
    int count() const override;
    QLayoutItem* itemAt(int index) const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect& rect) override;
    QSize sizeHint() const override;
    QLayoutItem* takeAt(int index) override;
private:
    int doLayout(const QRect& rect, bool testOnly) const;
    int smartSpacing(QStyle::PixelMetric pm) const;

    QList<QLayoutItem*> itemList;
    int m_hSpace;
    int m_vSpace;


};