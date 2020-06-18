
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

    def __init__(self, year, isdata):

        self.isdata = isdata
        self.smear_params = SmearParams(*(
                {'2016': [1.013, 0.014,
                          0.029, 0.047],
                 '2017': [1.017, 0.021,
                          0.058, 0.066],
                 '2018': [0.985, 0.019,
                          0.080, 0.073],
                 }[year]
                ))

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
                (max(nominal - band, no_smear), nominal + band) \
                if regressed > gen_pt else \
                (min(nominal + band, no_smear), nominal - band)

            return SmearResult(down, nominal, up)

        return SmearResult(no_smear, no_smear, no_smear)
