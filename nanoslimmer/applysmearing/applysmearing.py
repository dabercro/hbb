
import math


class SmearParams:
    def __init__(self, scale, scale_err, smear, smear_err):
        self.scale = scale
        self.scale_err = scale_err
        self.smear = smear
        self.smear_err = smear_err

class SmearResult:
    def __init__(self, down, nominal, up):
        self.down = down
        self.nominal = nominal
        self.up = up

class SmearApplicator:

    def __init__(self, year, isdata, useV13, doscaling):

        self.isdata = isdata
        self.smear_params = SmearParams(*(
                ({'2016': [0.998, 0.019,
                           0.017, 0.060],
                  '2017': [1.020, 0.023,
                           0.088, 0.071],
                  '2018': [0.985, 0.019,
                           0.080, 0.073],
                  } if useV13 else
                 {'2016': [1.004, 0.018,
                          -0.044, 0.061],
                  '2017': [1.011, 0.022,
                           0.051, 0.068],
                  '2018': [0.982, 0.019,
                           0.150, 0.079],
                  })[year]
                ))

        if not doscaling:
            self.smear_params.scale = 1.0

    def get_smear(self, jet_pt, regression_factor, gen_pt, rho=0): # Not using rho yet

        regressed = jet_pt * regression_factor

        if self.isdata:
            return SmearResult(regressed, regressed, regressed)

        # Scale MC
        no_smear = regressed * self.smear_params.scale

        if gen_pt:
            gen_diff = regressed - gen_pt
            nominal = max(0.0, (gen_pt + gen_diff * (1.0 + self.smear_params.smear)) * self.smear_params.scale)
            band = math.sqrt(pow(nominal/self.smear_params.scale * self.smear_params.scale_err, 2) +
                             pow(gen_diff * self.smear_params.scale * self.smear_params.smear_err, 2))

            down, up = \
                (nominal - band, nominal + band) \
                if regressed > gen_pt else \
                (nominal + band, nominal - band)

            return SmearResult(down, nominal, up)

        return SmearResult(no_smear, no_smear, no_smear)
