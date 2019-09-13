#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(39);
	ofSetLineWidth(2);
	ofEnableDepthTest();


}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
	if (ofGetFrameNum() % 60 < 45) {

		this->noise_seed += ofMap(ofGetFrameNum() % 60, 0, 45, 0.02, 0);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	auto span = 300;
	auto range = 150;
	auto radius = 50;

	this->cam.begin();
	
	for (int x = -span; x <= span; x += span) {

		auto parent_location = glm::vec3(x, 0, 0);

		ofNoFill();
		ofDrawCircle(parent_location, radius);

		vector<int> deg_list(360, 0);
		for (int i = 0; i < 5; i++) {

			auto child_location = glm::vec3(
				x + ofMap(ofNoise(ofRandom(10000), this->noise_seed), 0, 1, -range, range),
				ofMap(ofNoise(ofRandom(10000), this->noise_seed), 0, 1, -range, range),
				0
			);

			auto distance = (glm::distance(parent_location, child_location) - radius) * 3;
			auto rad = std::atan2(child_location.y - parent_location.y, child_location.x - parent_location.x);
			auto line_location = parent_location + glm::vec2(radius * cos(rad), radius * sin(rad));

			ofDrawLine(child_location, line_location);
			ofFill();
			ofDrawSphere(child_location, radius * 0.1);

			int deg = rad * RAD_TO_DEG;
			auto deg_span = 45;
			for (int tmp_deg = deg - deg_span * 0.5; tmp_deg <= deg + deg_span * 0.5; tmp_deg++) {

				tmp_deg < 0 ? deg_list[tmp_deg + 360] += distance : deg_list[tmp_deg] += distance;
			}
		}

		ofFill();
		ofSetColor(39);
		vector<glm::vec3> vertices;
		for (int deg = 0; deg < 360; deg++) {

			ofBeginShape();
			ofVertex(parent_location + glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), 0));
			ofVertex(parent_location + glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), deg_list[deg]));
			ofVertex(parent_location + glm::vec3(radius * cos(((deg + 1) % 360) * DEG_TO_RAD), radius * sin(((deg + 1) % 360) * DEG_TO_RAD), deg_list[((deg + 1) % 360)]));
			ofVertex(parent_location + glm::vec3(radius * cos(((deg + 1) % 360) * DEG_TO_RAD), radius * sin(((deg + 1) % 360) * DEG_TO_RAD), 0));
			ofEndShape();

		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}