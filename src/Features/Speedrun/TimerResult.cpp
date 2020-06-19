#include "TimerResult.hpp"

#include <cstring>

#include "TimerSplit.hpp"

#include "Modules/Console.hpp"

#include "SpeedrunTimer.hpp"
#include "Llanfair.hpp"

TimerResult::TimerResult()
    : total(0)
    , curSplit(nullptr)
    , prevSplit(nullptr)
    , splits()
{
}
void TimerResult::NewSplit(const int started, const char* map)
{
    this->prevSplit = this->curSplit;
    this->curSplit = new TimerSplit(started, map);
}
void TimerResult::EndSplit(const int finished)
{
    this->total = finished;
    this->curSplit->finished = finished;
    this->splits.push_back(this->curSplit);
}
void TimerResult::Split(const int ticks, const char* map)
{
    // console->Print("Old split started at %i ticks\n", this->curSplit->GetTotal());
    this->EndSplit(ticks);
    // llanfair->SendMsg(3, ticks, speedrun->GetIntervalPerTick());
    console->Print("(TimerResult) Split at %i ticks\n", ticks);
    // console->Print("Interval per tick = %f\n", speedrun->GetIntervalPerTick());
    this->NewSplit(ticks, map);
    // console->Print("Old split total: %i\n", this->prevSplit->GetTotal());
}
void TimerResult::AddSegment(int ticks)
{
    this->curSplit->segments.push_back(TimerSegment{ ticks });
}
void TimerResult::UpdateSplit(const char* map)
{
    for (const auto& split : this->splits) {
        if (!std::strcmp(map, split->map)) {
            this->prevSplit = this->curSplit;
            this->curSplit = split;
        }
    }
}
void TimerResult::Reset()
{
    this->total = 0;
    this->prevSplit = nullptr;

    auto deleted = false;
    for (auto split = this->splits.begin(); split != this->splits.end();) {
        if (*split == this->curSplit) {
            deleted = true;
        }
        delete *split;
        split = this->splits.erase(split);
    }

    if (!deleted && this->curSplit) {
        delete this->curSplit;
        this->curSplit = nullptr;
    }
}
TimerResult::~TimerResult()
{
    this->Reset();
}
