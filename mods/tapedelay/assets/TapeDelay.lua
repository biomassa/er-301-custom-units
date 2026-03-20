local app = app
local Class = require "Base.Class"
local Unit = require "Unit"
local GainBias = require "Unit.ViewControl.GainBias"
local Encoder = require "Encoder"

local tapedelay = require "tapedelay.libtapedelay"

local TapeDelay = Class {}
TapeDelay:include(Unit)

function TapeDelay:init(args)
  args.title = "Tape Delay"
  args.mnemonic = "TD"
  Unit.init(self, args)
end

function TapeDelay:onLoadGraph(channelCount)
  local stereo = channelCount > 1
  self.isStereo = stereo
  
  local delay = self:addObject("delay", tapedelay.TapeDelay(stereo))
  
  local mix = self:addObject("mix", app.GainBias())
  local time = self:addObject("time", app.GainBias())
  local timeRange = self:addObject("timeRange", app.MinMax())
  
  local timeR = nil
  local timeRangeR = nil
  if stereo then
    timeR = self:addObject("timeR", app.GainBias())
    timeRangeR = self:addObject("timeRangeR", app.MinMax())
  end

  local feedback = self:addObject("feedback", app.GainBias())
  local wow = self:addObject("wow", app.GainBias())
  local flutter = self:addObject("flutter", app.GainBias())
  local drive = self:addObject("drive", app.GainBias())

  connect(mix, "Out", delay, "Mix")
  connect(time, "Out", delay, "Time L")
  connect(time, "Out", timeRange, "In")
  
  if stereo then
    connect(timeR, "Out", delay, "Time R")
    connect(timeR, "Out", timeRangeR, "In")
  end

  connect(feedback, "Out", delay, "Feedback")
  connect(wow, "Out", delay, "Wow")
  connect(flutter, "Out", delay, "Flutter")
  connect(drive, "Out", delay, "Drive")

  self:addMonoBranch("mix", mix, "In", mix, "Out")
  self:addMonoBranch("time", time, "In", time, "Out")
  if stereo then
    self:addMonoBranch("timeR", timeR, "In", timeR, "Out")
  end
  self:addMonoBranch("feedback", feedback, "In", feedback, "Out")
  self:addMonoBranch("wow", wow, "In", wow, "Out")
  self:addMonoBranch("flutter", flutter, "In", flutter, "Out")
  self:addMonoBranch("drive", drive, "In", drive, "Out")

  connect(self, "In1", delay, "In1")
  connect(delay, "Out1", self, "Out1")
  
  if stereo then
    connect(self, "In2", delay, "In2")
    connect(delay, "Out2", self, "Out2")
  end
end

function TapeDelay:onLoadViews(objects, branches)
  local controls = {}
  local views = {
    expanded = {"mix", "time"},
    collapsed = {}
  }
  
  controls.mix = GainBias {
    button = "mix",
    description = "Wet/Dry Mix",
    branch = branches.mix,
    gainbias = objects.mix,
    range = objects.mix,
    biasMap = Encoder.getMap("[0,1]"),
    initialBias = 0.5,
    gainMap = Encoder.getMap("gain")
  }

  controls.time = GainBias {
    button = self.isStereo and "time L" or "time",
    description = self.isStereo and "Delay Time L" or "Delay Time",
    branch = branches.time,
    gainbias = objects.time,
    range = objects.timeRange,
    biasMap = Encoder.getMap("unit"),
    biasUnits = app.unitSecs,
    initialBias = 0.5,
    gainMap = Encoder.getMap("gain")
  }

  if self.isStereo then
    table.insert(views.expanded, "timeR")
    controls.timeR = GainBias {
      button = "time R",
      description = "Delay Time R",
      branch = branches.timeR,
      gainbias = objects.timeR,
      range = objects.timeRangeR,
      biasMap = Encoder.getMap("unit"),
      biasUnits = app.unitSecs,
      initialBias = 0.5,
      gainMap = Encoder.getMap("gain")
    }
  end

  table.insert(views.expanded, "feedback")
  controls.feedback = GainBias {
    button = "fdbk",
    description = "Feedback Level",
    branch = branches.feedback,
    gainbias = objects.feedback,
    range = objects.feedback,
    biasMap = Encoder.getMap("[-1,1]"),
    initialBias = 0.3,
    gainMap = Encoder.getMap("gain")
  }

  table.insert(views.expanded, "wow")
  controls.wow = GainBias {
    button = "wow",
    description = "Wow Modulation",
    branch = branches.wow,
    gainbias = objects.wow,
    range = objects.wow,
    biasMap = Encoder.getMap("[0,1]"),
    initialBias = 0.1,
    gainMap = Encoder.getMap("gain")
  }

  table.insert(views.expanded, "flutter")
  controls.flutter = GainBias {
    button = "flut",
    description = "Flutter Modulation",
    branch = branches.flutter,
    gainbias = objects.flutter,
    range = objects.flutter,
    biasMap = Encoder.getMap("[0,1]"),
    initialBias = 0.05,
    gainMap = Encoder.getMap("gain")
  }

  table.insert(views.expanded, "drive")
  controls.drive = GainBias {
    button = "drv",
    description = "Tape Drive",
    branch = branches.drive,
    gainbias = objects.drive,
    range = objects.drive,
    -- Drive unipolar [0, 1] per requested feedback
    biasMap = Encoder.getMap("[0,1]"),
    initialBias = 0.0,
    gainMap = Encoder.getMap("gain")
  }

  return controls, views
end

return TapeDelay
