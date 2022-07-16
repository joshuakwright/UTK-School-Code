import random
import threading
import time
import timeit

lock = threading.Lock()

def runnerTime(runner):
    return runner.overallTime

class Runner(threading.Thread):
    def __init__(self, contestantNumber):
        threading.Thread.__init__(self)
        self.contestantNumber = contestantNumber
        self.currSection = 0
        self.prevSecTime = 0
        self.overallTime = 0
        self.currSectionTime = 0

    def run(self):
        for i in range(0, 4):
            self.currSection = i + 1
            if (i != 0):
                self.overallTime += self.prevSecTime
            self.prevSecTime = random.uniform(10.00000000, 100.00000001)
            time.sleep(self.prevSecTime)
        self.overallTime += self.prevSecTime
        self.currSection = 'END'

class Reporter(threading.Thread):
    def __init__(self, runners, startTime, completeRunners):
        threading.Thread.__init__(self)
        self.currRunners = runners
        self.completedRunners = completeRunners
        self.raceStartTime = startTime
        self.loopStartTime = 0

    def run(self):
        outputFile = open("ReporterUpdates.txt", "w")
        while (self.currRunners):
            print("Results at time:", self.loopStartTime, file=outputFile)
            print("----------------------------------------------------------", file=outputFile)
            print("| Contestant  #  | Section | Section Time | Overall Time |", file=outputFile)
            timeFromStart = timeit.default_timer() - self.raceStartTime
            runnersToRemove = list()
            for runner in self.currRunners:
                if(runner.currSection == 'END'):
                    runnersToRemove.append(runner)
                print("| Contestant ", end='', file=outputFile)
                print(str(runner.contestantNumber).rjust(3, '0'), end='', file=outputFile)
                print(" |  ", end='', file=outputFile)
                if runner.currSection == 'END':
                    print(" End ", end='', file=outputFile)
                    self.completedRunners.append(runner)
                else:
                    print(" ", str(runner.currSection), " ", end='', file=outputFile)
                print("  | ", end='', file=outputFile)
                if runner.currSection == 'END':
                    print("%-12.8f" % runner.prevSecTime, end='', file=outputFile)
                elif (runner.currSection != 1):
                    print("%-12.8f" % (timeFromStart - runner.overallTime), end='', file=outputFile)
                else:
                    print("%-12.8f" % timeFromStart, end='', file=outputFile)
                print(" | ", end='', file=outputFile)
                if(runner.currSection == 'END'):
                    print("%-12.8f" % runner.overallTime, end='', file=outputFile)
                else:
                    print("%-12.8f" % timeFromStart, end='', file=outputFile)
                print(" |", file=outputFile)
            print("----------------------------------------------------------", file=outputFile)

            for runner in runnersToRemove:
                self.currRunners.remove(runner)

            self.loopStartTime += 2
            time.sleep(2)
        outputFile.close()

        finalFile = open("FinalResults.txt", "w")
        print("---------------------------------", file=finalFile)
        print("| Contestant  #  | Overall Time |", file=finalFile)
        totalTime = 0

        self.completedRunners.sort(key=runnerTime)
        for runner in self.completedRunners:
            print("| Contestant ", end='', file=finalFile)
            print(str(runner.contestantNumber).rjust(3, '0'), end='', file=finalFile)
            print(" | ", end='', file=finalFile)
            totalTime += runner.overallTime
            print("%-12.8f" % runner.overallTime, end='', file=finalFile)
            print(" |", file=finalFile)
        print("---------------------------------", file=finalFile)
        print("Overall, the runners spent ", end='', file=finalFile)
        print("%.8f" % totalTime, end='', file=finalFile)
        print(" seconds running total.", file=finalFile)
        finalFile.close()

if __name__ == '__main__':
    currRunners = list()
    completedRunners = list()
    startTime = timeit.default_timer()

    for i in range(0, 1000):
        runner = Runner(i + 1)
        currRunners.append(runner)
        runner.start()

    reporter = Reporter(currRunners, startTime, completedRunners)
    reporter.start()
    reporter.join()
