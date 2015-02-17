<?hh

namespace DateUtils;

/**
 * Class WorkingDays
 * @package DateUtils
 */
class WorkingDays
{
    /**
     * @var array
     */
    protected array<string> $config;

    public function __construct(array<string> $config)
    {
        $this->config = $config;
    }

    /**
     * @param \DateTime $initialDate
     * @param int $workingDayOffset
     * @return \DateTime
     * @throws \LogicException
     */
    public function workingDaysFrom(?\DateTime $initialDate = null, int $workingDayOffset = 1) : \DateTime
    {
        if (null === $initialDate) {
            $initialDate = new \DateTime();
        }

        if ($workingDayOffset < 0) {
            throw new \LogicException('Cannot calculate working days on a negative offset.');
        }

        $dayCounter = 1;
        $currentDay = $initialDate->getTimestamp();
        $holidays = $this->getBankHolidays((int)$initialDate->format('Y'));

        while ($dayCounter <= $workingDayOffset) {

            $date = date('Y-m-d', $currentDay);
            $currentDay = strtotime($date . ' +1 day');

            if (date('Y', $currentDay) != $initialDate->format('Y')) {
                $holidays = $this->getBankHolidays((int)date('Y', $currentDay));
            }

            $date = date('Y-m-d', $currentDay);
            $weekday = date('N', $currentDay);

            if ($weekday < 6 && !in_array($date, $holidays)) {
                $dayCounter++;
            }
        }

        return $initialDate->setTimestamp($currentDay);
    }

    /**
     * @param  \DateTime $initialDate
     * @param  \DateInterval $workingDayOffset
     * @return \DateTime
     */
    public function workingDaysFromInterval(?\DateTime $initialDate = null, ?\DateInterval $workingDayOffset = null)
    {
        if (null === $initialDate) {
            $initialDate = new \DateTime();
        }

        return $this->workingDaysFromOffset($initialDate, new \DateInterval($workingDayOffset));
    }


    /**
     * If the offset is greater than 1 day or it forces us to rollover convert it back to an int and call the
     * other function
     * @param \DateTime $initialDate
     * @param \DateInterval $offset
     * @return \DateTime
     */
    protected function workingDaysFromOffset(\DateTime $initialDate, \DateInterval $offset) : \DateTime
    {
        $targetDate = new \DateTime($initialDate->format(\DateTime::ISO8601));
        $diffDate = \DateTime::createFromFormat('Y-m-d h:i:s', $initialDate->format('Y-m-d 00:00:00'));
        $difference = $targetDate->add($offset)->diff($diffDate);

        if ($difference->days >= 1) {
            return $this->workingDaysFrom($initialDate, $difference->days);
        }

        return $initialDate->add($offset);
    }

    /**
     * @param \DateTime $targetDate
     * @return bool
     */
    public function isWorkingDay(\DateTime $targetDate) : bool
    {
        $holidays = $this->getBankHolidays((int)$targetDate->format('Y'));
        $weekDays = range(1, 5);


        $workingDay = (
            !in_array($targetDate->format('N'), $weekDays) ||
            in_array($targetDate->format('Y-m-d'), $holidays)
        );

        return (bool)!$workingDay;
    }

    /**
     * Alias function to calculate days from today, which is the most common usage
     * @param int $workingDayOffset
     * @return \DateTime
     */
    public function workingDaysFromToday(int $workingDayOffset = 1) : \DateTime
    {
        return $this->workingDaysFrom(null, $workingDayOffset);
    }

    /**
     * Alias function to calculate days from today, which is the most common usage
     * @param \DateInterval $workingDayOffset
     * @return \DateTime
     */
    public function workingDayFromTodayUsingInterval(\DateInterval $workingDayOffset) : \DateTime
    {
        return $this->workingDaysFromInterval(null, $workingDayOffset);
    }

    /**
     * @param \DateTime $startDay
     * @param \DateTime $endDay
     * @return int
     */
    public function workingDaysBetween(\DateTime $startDay, \DateTime $endDay) : int
    {
        $holidays = $this->getBankHolidays((int)$startDay->format('Y'));
        $weekDays = range(1, 5);

        $interval = new \DateInterval('P1D');
        $periods = new \DatePeriod($startDay, $interval, $endDay);

        $days = 0;

        foreach ($periods as $period) {

            if ($period->format('Y') != $startDay->format('Y')) {
                $holidays = $this->getBankHolidays((int)$period->format('Y'));
            }

            if (!in_array($period->format('N'), $weekDays)) {
                continue;
            }
            if (in_array($period->format('Y-m-d'), $holidays)) {
                continue;
            }
            $days++;
        }

        return $days;
    }

    /**
     * @param  int $year
     * @return array
     */
    protected function getBankHolidays(int $year)
    {
        $bankHolidays = new BankHolidays($this->config, $year);

        return $bankHolidays->getBankHolidays();
    }
}
