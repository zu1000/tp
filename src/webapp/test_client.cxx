#include "test_client.hpp"
#include "message_event.hpp"

#include <Wt/WTime>
#include <Wt/Chart/WDataSeries>
#include <Wt/Chart/WCartesianChart>

#include <boost/pointer_cast.hpp>

using namespace Wt;
using namespace Wt::Chart;

using namespace TP::Web;

namespace TP {
namespace WebApp {

TestClient::TestClient(
        const Wt::WEnvironment& env,
        ExternalEventManager& external_event_manager)
    : WApplication(env)
    , external_event_manager_(external_event_manager)
    , model_(0)
{
    setTitle("TP demo client");
    root()->addWidget(&text_);

    ExternalEventHandler handler = boost::bind(&TestClient::handleMessageEvent, this, _1);

    ExternalEventListener listener(sessionId(), handler);

    external_event_manager.addExternalEventListener(ExtMessageEvent, listener);

    external_event_manager.start();

    createChart();

    if (!updatesEnabled())
        enableUpdates();
}

TestClient::~TestClient()
{
    external_event_manager_.delExternalEventListener(ExtMessageEvent, sessionId());
}

void
TestClient::handleMessageEvent(const EventPtr& event)
{
    const ExternalMessageEvent& evt =
        *boost::static_pointer_cast<ExternalMessageEvent>(event);

    text_.setText(evt.getEventData().message());

    model_->insertRow(model_->rowCount());
    model_->setData(model_->rowCount() - 1, 0, model_->rowCount() - 1);
    model_->setData(model_->rowCount() - 1, 1, boost::any(300.50));

    WApplication::instance()->triggerUpdate();
}

void
TestClient::createChart()
{
    model_ = new WStandardItemModel(0, 0, this);

    for (int i = 0; i < 2; ++i)
    {
        model_->insertColumn(i);
        if (i == 0)
            model_->setHeaderData(i, WString("Ticker Time"));
        else
            model_->setHeaderData(i, WString("Ticker Data"));
    }

    WTime time(12, 0, 0);
    double tick = 101.00;
    for (int i = 0; i < 100; ++i)
    {
        model_->insertRow(i);
        //time.addSecs(1);
        tick += i*0.1;
        model_->setData(i, 0, i);
        model_->setData(i, 1, boost::any(tick));
    }

    WCartesianChart* chart = new WCartesianChart();
    chart->setModel(model_);
    chart->setXSeriesColumn(0);
    chart->setLegendEnabled(true);

    chart->setType(ScatterPlot);

    chart->setPlotAreaPadding(80, Left);
    chart->setPlotAreaPadding(40, Top | Bottom);

    for (int i = 1; i < model_->columnCount(); ++i)
    {
        WDataSeries s(i, LineSeries);
        s.setShadow(WShadow(3, 3, WColor(0, 0, 0, 127), 3));
        chart->addSeries(s);
    }

    chart->resize(800, 400);

    root()->addWidget(chart);
}

}}
