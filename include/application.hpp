#pragma once

namespace vm {
	class ApplicationI {
	private:
		virtual void initianize() = 0;
		virtual void run() = 0;
		virtual void uninitialize() = 0;
	};
}