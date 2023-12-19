#include "JoePath.h"
#include "../DrawUtils.h"
#include "../../Utils/Logger.h"
JoePath::JoePath(const std::vector<JoeSegment>& segments, bool isIncomplete) : segments(segments), isIncomplete(isIncomplete) {

}
JoePath::JoePath() : segments(), isIncomplete(false) {
}
void JoePath::draw(int highlight) const {
	DrawUtils::SetColor(MC_Color(0.f, 1.f, 0.f, 1.f));
	bool changedColor = false;
	for(int i = 0; i < segments.size(); i++){
		auto seg = segments[i];

		if(i == highlight){
			DrawUtils::SetColor(MC_Color(0.f, 1.f, 0.f, 1.f));
			changedColor = true;
		}else if(changedColor)
			DrawUtils::SetColor(MC_Color(1.f, 0.f, 0.f, 1.f));
		seg.draw();
	}
}
bool JoePath::isIncomplete1() const {
	return isIncomplete;
}
void JoePath::cutoff(float percentageKeep) {
	if(this->getNumSegments() == 0)
		return;
	int numKeep = (int)ceilf(this->getNumSegments() * percentageKeep);
	this->segments.erase(this->segments.begin() + numKeep, this->segments.end());
}
void JoePath::initPathSegments() {
	// Init segments
	for(auto& seg : segments){
		seg.init();
	}

	// Check whether we can sprint or not
	if(segments.size() > 1){

		for(int i = 0; i < segments.size() - 2; i++){
			auto& curSeg = segments.at(i);
			auto& nextSeg = segments.at(i + 1);
			if(curSeg.getSegmentType() != JoeSegmentType::WALK || nextSeg.getSegmentType() != JoeSegmentType::WALK){
				curSeg.setAllowSprint(false);
				continue;
			}
			if(curSeg.getStart().y != curSeg.getEnd().y){
				curSeg.setAllowSprint(false);
				continue;
			}

			auto currentTangent = curSeg.getEnd().sub(curSeg.getStart()).toFloatVector().normalize();
			auto nextTangent = nextSeg.getEnd().sub(nextSeg.getStart()).toFloatVector().normalize();
			if(currentTangent.dot(nextTangent) > 0.3f){ // make sure we're running in a similar direction
				curSeg.setAllowSprint(true);
			}
		}
	}

	this->isInitialized = true;
}
bool JoePath::isInitialized1() const {
	return isInitialized;
}
