#pragma once

#include <QGraphicsView>
#include <QWheelEvent>
#include "Core/ICore.hpp"


/** \class StoryView
 *  \brief Класс предназначен для отображения сцены нодов истории.
 */
class StoryView : public QGraphicsView
{
public:
    explicit StoryView(QWidget* parent = Q_NULLPTR);
    virtual ~StoryView();

    /** \brief Установка коры
     */
    void setCore(ICorePtr core);

    /** \brief Установка зума (zoom = 2^x)
     */
    void setZoom(double zoom);

protected:
    /** \brief Обработка событий колесика мыши
     */
    void wheelEvent (QWheelEvent* event) override;

    /** \brief Обработка событий нажатия мыши
     */
    void mousePressEvent(QMouseEvent* event) override;

    /** \brief Обработка событий перемещения мыши
     */
    void mouseMoveEvent(QMouseEvent* event) override;

private:

    /** \brief Зум +
     */
    void zoomIn();

    /** \brief Зум -
     */
    void zoomOut();

    /** \brief Установить нулевой зум
     */
    void unZoom();

    /** \brief Задать новую матрицу масштабирования
     */
    void setupMatrix();

    const double m_zoomMax  = 0;    /**< Максимально допустимый масштаб (Увеличение). */
    const double m_zoomMin  = -3;   /**< Минимально допустимый масштаб (Уменьшение). */
    const double m_zoomStep = 0.1;  /**< Шаг масштабирования. */
    double m_currZoom;              /**< Текущий масштаб. */
    ICorePtr m_core;                /**< Указатель на кору. */

    double m_originX;               /**< X координата (для перемещения по сцене). */
    double m_originY;               /**< Y координата (для перемещения по сцене). */
};
