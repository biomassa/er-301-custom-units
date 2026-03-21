local lib = require "granulator.libgranulator"
local Class = require "Base.Class"
local Unit = require "Unit"
local GainBias = require "Unit.ViewControl.GainBias"
local OptionControl = require "Unit.ViewControl.OptionControl"
local Encoder = require "Encoder"

local granulator = Class {}
granulator:include(Unit)

function granulator:init(args)
  args.title = "Granulator"
  args.mnemonic = "GR"
  Unit.init(self, args)
end

function granulator:onLoadGraph(channelCount)
  local stereo = channelCount > 1

  -- Create DSP instance(s)
  local gran1 = self:addObject("gran1", lib.gran())

  -- Shared parameter adapters
  local sizeAmount = self:addObject("size", app.ParameterAdapter())
  local speedAmount = self:addObject("speed", app.ParameterAdapter())
  local probAmount = self:addObject("prob", app.ParameterAdapter())

  -- Connect parameters to first instance
  tie(gran1, "GrainSize", sizeAmount, "Out")
  tie(gran1, "Speed", speedAmount, "Out")
  tie(gran1, "Probability", probAmount, "Out")

  -- Mono branches for modulation
  self:addMonoBranch("sizeBranch", sizeAmount, "In", sizeAmount, "Out")
  self:addMonoBranch("speedBranch", speedAmount, "In", speedAmount, "Out")
  self:addMonoBranch("probBranch", probAmount, "In", probAmount, "Out")

  -- Channel 1 audio routing
  connect(self, "In1", gran1, "In")
  connect(gran1, "Out", self, "Out1")

  if stereo then
    -- Second independent DSP instance for right channel
    local gran2 = self:addObject("gran2", lib.gran())

    -- Connect shared parameters to second instance
    tie(gran2, "GrainSize", sizeAmount, "Out")
    tie(gran2, "Speed", speedAmount, "Out")
    tie(gran2, "Probability", probAmount, "Out")

    -- Channel 2 audio routing
    connect(self, "In2", gran2, "In")
    connect(gran2, "Out", self, "Out2")
  end
end

function granulator:onLoadViews(objects, branches)
  local controls = {}
  local views = {
    expanded = { "env", "sz", "sp", "pr" },
    collapsed = {},
  }

  local self2 = self
  controls.env = OptionControl {
    button = "env",
    description = "Envelope Shape",
    option = objects.gran1:getOption("EnvShape"),
    choices = { "Hann", "Blackman", "Triangle", "Tukey" },
    muteOnChange = true,
    onUpdate = function(choice)
      if self2.objects.gran2 then
        local val = objects.gran1:getOption("EnvShape"):value()
        self2.objects.gran2:getOption("EnvShape"):set(val)
      end
    end
  }

  local sizeMap = app.LinearDialMap(0.001, 2.0)
  sizeMap:setSteps(0.01, 0.001, 0.0001, 0.00001)

  local speedMap = app.LinearDialMap(1, 400)
  speedMap:setSteps(10, 1, 0.25, 0.25)
  speedMap:setRounding(1)

  local probMap = app.LinearDialMap(0.0, 1.0)
  probMap:setSteps(0.1, 0.01, 0.001, 0.0001)

  controls.sz = GainBias {
    button = "size",
    description = "Grain Size",
    branch = branches.sizeBranch,
    gainbias = objects.size,
    range = objects.size,
    biasUnits = app.unitSecs,
    biasMap = sizeMap,
    initialBias = 0.2,
  }

  controls.sp = GainBias {
    button = "spd",
    description = "Speed",
    branch = branches.speedBranch,
    gainbias = objects.speed,
    range = objects.speed,
    biasUnits = app.unitHertz,
    biasMap = speedMap,
    initialBias = 10,
  }

  controls.pr = GainBias {
    button = "prb",
    description = "Probability",
    branch = branches.probBranch,
    gainbias = objects.prob,
    range = objects.prob,
    biasUnits = app.unitNone,
    biasMap = probMap,
    initialBias = 0.7,
  }

  return controls, views
end

return granulator
